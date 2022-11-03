#include "register_types.h"

#include "core/class_db.h"

#include "db.h"

#include "job.h"
#include "job_manager.h"
#include "ticket.h"

#include "http.h"
#include "net_request.h"
#include "anitya_http.h"

void register_anitya_server_types()
{
    ClassDB::register_class<LocalAssetData>();
    ClassDB::register_class<IDB>();

    ClassDB::register_class<IHTTP>();
    ClassDB::register_class<AnityaHTTP>();
    ClassDB::register_class<NetRequest>();
    ClassDB::register_class<NetRequestResponse>();
    ClassDB::register_class<NetRequestData>();


    ClassDB::register_class<JobResult>(); 
    ClassDB::register_class<AJob>();
    ClassDB::register_class<JobManager>();
    ClassDB::register_class<Ticket>();
}

void unregister_anitya_server_types() {}
