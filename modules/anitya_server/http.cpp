#include "http.h"

void IHTTP::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("make_request", "request_data"), &IHTTP::make_request);
    ClassDB::bind_method(D_METHOD("update"), &IHTTP::update);
}