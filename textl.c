#include "common.h"
#include "except.h"
#include "u4c_priv.h"

using namespace std;

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void
u4c_text_listener_t::begin()
{
    nrun_ = 0;
    nfailed_ = 0;
    fprintf(stderr, "u4c: running\n");
}

void
u4c_text_listener_t::end()
{
    fprintf(stderr, "u4c: %u run %u failed\n",
	    nrun_, nfailed_);
}

void
u4c_text_listener_t::begin_node(const u4c_testnode_t *tn)
{
    fprintf(stderr, "u4c: running: \"%s\"\n", tn->get_fullname().c_str());
    result_ = u4c::R_UNKNOWN;
}

void
u4c_text_listener_t::end_node(const u4c_testnode_t *tn)
{
    string fullname = tn->get_fullname();

    nrun_++;
    switch (result_)
    {
    case u4c::R_PASS:
	fprintf(stderr, "PASS %s\n", fullname.c_str());
	break;
    case u4c::R_NOTAPPLICABLE:
	fprintf(stderr, "N/A %s\n", fullname.c_str());
	break;
    case u4c::R_FAIL:
	nfailed_++;
	fprintf(stderr, "FAIL %s\n", fullname.c_str());
	break;
    default:
	fprintf(stderr, "??? (result %d) %s\n", result_, fullname.c_str());
	break;
    }
}

void
u4c_text_listener_t::add_event(const u4c_event_t *ev, u4c::functype_t ft)
{
    const char *type;
    char buf[2048];

    switch (ev->which)
    {
    case EV_ASSERT: type = "ASSERT"; break;
    case EV_EXIT: type = "EXIT"; break;
    case EV_SIGNAL: type = "SIGNAL"; break;
    case EV_SYSLOG: type = "SYSLOG"; break;
    case EV_FIXTURE: type = "FIXTURE"; break;
    case EV_EXPASS: type = "EXPASS"; break;
    case EV_EXFAIL: type = "EXFAIL"; break;
    case EV_EXNA: type = "EXNA"; break;
    case EV_VALGRIND: type = "VALGRIND"; break;
    case EV_SLMATCH: type = "SLMATCH"; break;
    default: type = "unknown"; break;
    }
    snprintf(buf, sizeof(buf), "EVENT %s %s",
		type, ev->description);
    if (*ev->filename && ev->lineno)
	snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),
		 " at %s:%u",
		 ev->filename, ev->lineno);
    if (*ev->function)
	snprintf(buf+strlen(buf), sizeof(buf)-strlen(buf),
		 " in %s %s",
		 as_string(ft),
		 ev->function);
    strcat(buf, "\n");
    fputs(buf, stderr);
}

void
u4c_text_listener_t::finished(u4c::result_t res)
{
    result_ = res;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
