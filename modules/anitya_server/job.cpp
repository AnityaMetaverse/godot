#include "job.h"

// void JobResult::_init(bool p_succeed, const Dictionary& p_data)
// {
//     succeed = p_succeed;
//     data = p_data;
// }

void JobResult::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_succeed"), &JobResult::get_succeed);
    ClassDB::bind_method(D_METHOD("set_succeed", "succeed"), &JobResult::set_succeed);
    ClassDB::bind_method(D_METHOD("set_data", "data"), &JobResult::set_data);
    ClassDB::bind_method(D_METHOD("get_data"), &JobResult::get_data);
    // ClassDB::bind_method(D_METHOD("_init", "succeed", "data"), &JobResult::_init);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "succeed"), "set_succeed", "get_succeed");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "data"), "set_data", "get_data");
}


void AJob::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_priority"), &AJob::get_priority);
    ClassDB::bind_method(D_METHOD("set_priority", "priority"), &AJob::set_priority);

    ClassDB::bind_method(D_METHOD("get_scope"), &AJob::get_scope);
    ClassDB::bind_method(D_METHOD("set_scope", "scope"), &AJob::set_scope);

    ClassDB::bind_method(D_METHOD("get_duty"), &AJob::get_duty);
    ClassDB::bind_method(D_METHOD("set_duty", "duty"), &AJob::set_duty);


    ClassDB::bind_method(D_METHOD("start"), &AJob::start);
    ClassDB::bind_method(D_METHOD("update"), &AJob::update);
    ClassDB::bind_method(D_METHOD("done"), &AJob::done);
    ClassDB::bind_method(D_METHOD("is_done"), &AJob::is_done);
    ClassDB::bind_method(D_METHOD("set_job_result", "job_result"), &AJob::set_job_result);
    ClassDB::bind_method(D_METHOD("get_job_result"), &AJob::get_job_result);
    ClassDB::bind_method(D_METHOD("get_job_id"), &AJob::get_job_id);


    ADD_PROPERTY(PropertyInfo(Variant::INT, "scope"), "set_scope", "get_scope");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "duty"), "set_duty", "get_duty");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "priority"), "set_priority", "get_priority");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "result"), "set_job_result", "get_job_result");

    
    ADD_SIGNAL(MethodInfo("finished", PropertyInfo(Variant::OBJECT, "job"), PropertyInfo(Variant::OBJECT, "result")));
    // ADD_SIGNAL(MethodInfo("job_failed", PropertyInfo(Variant::OBJECT, "job")));

    BIND_ENUM_CONSTANT(JOB_DUTY_MUST);
    BIND_ENUM_CONSTANT(JOB_DUTY_MAYBE);

    BIND_ENUM_CONSTANT(JOB_PRIORITY_HIGH);
    BIND_ENUM_CONSTANT(JOB_PRIORITY_NORMAL);
    BIND_ENUM_CONSTANT(JOB_PRIORITY_LOW);
    
    BIND_ENUM_CONSTANT(JOB_SCOPE_LOCAL);
    BIND_ENUM_CONSTANT(JOB_SCOPE_REMOTE);


}

AJob::AJob(): result(memnew(JobResult)), status(memnew(JobStatus))
{

}

AJob::~AJob()
{
    // if (!result.is_null())
    // {
    //     memdelete(result.ptr());
    // }

    // if (!status.is_null())
    // {
    //     memdelete(status.ptr());
    // }
}