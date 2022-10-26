#include "anitya_http.h"

void AnityaHTTP::make_request(Ref<NetRequestData> p_data)
{
    Ref<NetRequest> r = Ref<NetRequest>(memnew(NetRequest));
    r->connect("request_finished", *(p_data->get_requester()), "_on_request_finished");
    r->start(p_data);
    requests.push_back(r);
}

void AnityaHTTP::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("make_request", "request_data"), &AnityaHTTP::make_request);
}