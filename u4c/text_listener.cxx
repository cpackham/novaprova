#include "u4c/text_listener.hxx"
#include "u4c/job.hxx"
#include "except.h"

namespace u4c {
using namespace std;

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void
text_listener_t::begin()
{
    nrun_ = 0;
    nfailed_ = 0;
    fprintf(stderr, "u4c: running\n");
}

void
text_listener_t::end()
{
    fprintf(stderr, "u4c: %u run %u failed\n",
	    nrun_, nfailed_);
}

void
text_listener_t::begin_job(const job_t *j)
{
    fprintf(stderr, "u4c: running: \"%s\"\n", j->as_string().c_str());
    result_ = u4c::R_UNKNOWN;
}

void
text_listener_t::end_job(const job_t *j)
{
    string nm = j->as_string();

    nrun_++;
    switch (result_)
    {
    case R_PASS:
	fprintf(stderr, "PASS %s\n", nm.c_str());
	break;
    case R_NOTAPPLICABLE:
	fprintf(stderr, "N/A %s\n", nm.c_str());
	break;
    case R_FAIL:
	nfailed_++;
	fprintf(stderr, "FAIL %s\n", nm.c_str());
	break;
    default:
	fprintf(stderr, "??? (result %d) %s\n", result_, nm.c_str());
	break;
    }
}

void
text_listener_t::add_event(const event_t *ev)
{
    string s = string("EVENT ") +
		ev->as_string() +
	       "\nCalled from\n" +
	       ev->get_long_location() +
	       "\n";
    fputs(s.c_str(), stderr);
}

void
text_listener_t::finished(result_t res)
{
    result_ = res;
}

// close the namespace
};
