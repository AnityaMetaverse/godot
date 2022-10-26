#include "register_types.h"

#include "core/class_db.h"

#include "job.h"
#include "job_manager.h"
#include "ticket.h"

void register_anitya_server_types()
{
    ClassDB::register_class<JobResult>(); 
    ClassDB::register_class<AJob>();
    ClassDB::register_class<JobManager>();
    ClassDB::register_class<Ticket>();
}

void unregister_anitya_server_types() {}
