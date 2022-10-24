#include "job.h"

void AJob::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("begin"), &AJob::begin);
    ClassDB::bind_method(D_METHOD("update"), &AJob::update);

    ClassDB::bind_method(D_METHOD("get_priority"), &AJob::get_priority);
    ClassDB::bind_method(D_METHOD("set_priority", "priority"), &AJob::set_priority);

    ClassDB::bind_method(D_METHOD("get_scope"), &AJob::get_scope);
    ClassDB::bind_method(D_METHOD("set_scope", "scope"), &AJob::set_scope);

    ClassDB::bind_method(D_METHOD("get_duty"), &AJob::get_duty);
    ClassDB::bind_method(D_METHOD("set_duty", "duty"), &AJob::set_duty);


    ADD_PROPERTY(PropertyInfo(Variant::INT, "scope"), "set_scope", "get_scope");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "duty"), "set_duty", "get_duty");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "priority"), "set_priority", "get_priority");

    
    ADD_SIGNAL(MethodInfo("job_finished", PropertyInfo(Variant::OBJECT, "job_result")));

}