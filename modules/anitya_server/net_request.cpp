#include "net_request.h"

void NetRequestData::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_requester"), &NetRequestData::get_requester);
    ClassDB::bind_method(D_METHOD("set_requester", "requester"), &NetRequestData::set_requester);

    ClassDB::bind_method(D_METHOD("get_method"), &NetRequestData::get_method);
    ClassDB::bind_method(D_METHOD("set_method", "method"), &NetRequestData::set_method);

    ClassDB::bind_method(D_METHOD("get_headers"), &NetRequestData::get_headers);
    ClassDB::bind_method(D_METHOD("set_headers", "headers"), &NetRequestData::set_headers);

    ClassDB::bind_method(D_METHOD("get_body"), &NetRequestData::get_body);
    ClassDB::bind_method(D_METHOD("set_body", "body"), &NetRequestData::set_body);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "requester", PROPERTY_HINT_OBJECT_ID), "set_requester", "get_requester");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "method", PROPERTY_HINT_TYPE_STRING), "set_method", "get_method");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "headers", PROPERTY_HINT_NONE), "set_headers", "get_headers");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_BYTE_ARRAY, "body", PROPERTY_HINT_NONE), "set_body", "get_body");
}

void NetRequestResponse::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_error"), &NetRequestResponse::get_error);
    ClassDB::bind_method(D_METHOD("set_error", "error"), &NetRequestResponse::set_error);

    ClassDB::bind_method(D_METHOD("get_data"), &NetRequestResponse::get_data);
    ClassDB::bind_method(D_METHOD("set_data", "data"), &NetRequestResponse::set_data);

    ClassDB::bind_method(D_METHOD("get_tag"), &NetRequestResponse::get_tag);
    ClassDB::bind_method(D_METHOD("set_tag", "tag"), &NetRequestResponse::set_tag);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "error", PROPERTY_HINT_NONE), "set_error", "get_error");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_BYTE_ARRAY, "data", PROPERTY_HINT_NONE), "set_data", "get_data");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "tag", PROPERTY_HINT_NONE), "set_tag", "get_tag");

}

void NetRequest::start(Ref<NetRequestData> p_data)
{
    request_data = p_data;
}

void NetRequest::update()
{

}

void NetRequest::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("start", "request_data"), &NetRequest::start);
    ClassDB::bind_method(D_METHOD("update"), &NetRequest::update);

    ADD_SIGNAL(MethodInfo("request_finished", PropertyInfo(Variant::OBJECT, "response")));
}