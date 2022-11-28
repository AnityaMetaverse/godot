#include "http.h"

void IHTTP::_bind_methods()
{
    BIND_VMETHOD(MethodInfo(Variant::BOOL, "make_request", PropertyInfo(Variant::OBJECT, "request")));
    // ClassDB::bind_method(D_METHOD("make_request", "request_data"), &IHTTP::make_request);
    // ClassDB::bind_method(D_METHOD("update"), &IHTTP::update);
    BIND_VMETHOD(MethodInfo("update"));
}