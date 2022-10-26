#include "ticket.h"

void Ticket::_init(Ref<AJob> p_job)
{
    job_id = p_job->call("get_job_id");
}

void Ticket::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_job_id"), &Ticket::get_job_id);
}