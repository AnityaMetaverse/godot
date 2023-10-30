#include "anitya_http.h"


int AnityaHTTP::_instances = 0;

bool AnityaHTTP::make_request(Ref<NetRequestData> p_data)
{
    if (p_data->get_callback_method() == "")
    {
        ERR_PRINT("Must provide a callback method");
        return false;
    }
    request_mutex.lock();

    Ref<NetRequest> r = Ref<NetRequest>(memnew(NetRequest));
    Error err = r->connect("request_finished", p_data->get_requester(), p_data->get_callback_method());
    if (err != OK)
    {
        ERR_PRINT(String("Cannot connect method: ") + String(p_data->get_callback_method()));
        return false;
    }
    if (!r->start(p_data))
    {
        request_mutex.unlock();
        return false;
    }

    requests.push_back(r);
    request_mutex.unlock();
    return true;

}

void AnityaHTTP::update()
{
    #ifndef ANITYA_ONE_NET_REQUEST
    request_mutex.lock();
    {
        for (int index = 0; index < requests.size(); index++)
        {
            Ref<NetRequest> r = requests.get(index);
            r->update();

            if (r->is_done())
            {
                Ref<NetRequestResponse> response = r->get_response();
                r->emit_signal("request_finished", response);
                requests.remove(index);
            }
        }
    }
    request_mutex.unlock();
    #else
    if (_current_request.ptr() == nullptr && requests.size() != 0)
    {
        request_mutex.lock();
        {
            const auto index = requests.size() - 1;
            _current_request = requests.get(index);
            requests.remove(index);
        }
        request_mutex.unlock();
    }
    else if (_current_request.ptr() != nullptr)
    {
        _current_request->update();
        if (_current_request->is_done())
        {
            Ref<NetRequestResponse> response = _current_request->get_response();
            _current_request->emit_signal("request_finished", response);
            _current_request.unref();
        }
    }
    #endif
}

void AnityaHTTP::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("make_request", "request_data"), &AnityaHTTP::make_request);
    ClassDB::bind_method(D_METHOD("update"), &AnityaHTTP::update);
}

AnityaHTTP::AnityaHTTP()
{
    ++_instances;
    if (_instances > 1)
    {
        ERR_PRINT("More than one AnityaHttp instance!");
    }
}

AnityaHTTP::~AnityaHTTP()
{
    --_instances;
}