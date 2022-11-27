#include "anitya_http.h"

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
}

void AnityaHTTP::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("make_request", "request_data"), &AnityaHTTP::make_request);
}