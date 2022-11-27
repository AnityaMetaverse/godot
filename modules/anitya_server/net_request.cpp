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

    ClassDB::bind_method(D_METHOD("get_url_params"), &NetRequestData::get_url_params);
    ClassDB::bind_method(D_METHOD("set_url_params", "url_params"), &NetRequestData::set_url_params);

    ClassDB::bind_method(D_METHOD("get_host"), &NetRequestData::get_host);
    ClassDB::bind_method(D_METHOD("set_host", "host"), &NetRequestData::set_host);

    ClassDB::bind_method(D_METHOD("get_callback_method"), &NetRequestData::get_callback_method);
    ClassDB::bind_method(D_METHOD("set_callback_method", "callback_method"), &NetRequestData::set_callback_method);

    ClassDB::bind_method(D_METHOD("get_tag"), &NetRequestData::get_tag);
    ClassDB::bind_method(D_METHOD("set_tag", "tag"), &NetRequestData::set_tag);

    ClassDB::bind_method(D_METHOD("get_port"), &NetRequestData::get_port);
    ClassDB::bind_method(D_METHOD("set_port", "port"), &NetRequestData::set_port);

    // ADD_PROPERTY(PropertyInfo(Variant::, "requester"), "set_requester", "get_requester");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "method", PROPERTY_HINT_NONE), "set_method", "get_method");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "headers", PROPERTY_HINT_NONE), "set_headers", "get_headers");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_BYTE_ARRAY, "body", PROPERTY_HINT_NONE), "set_body", "get_body");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "url_params", PROPERTY_HINT_NONE), "set_url_params", "get_url_params");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "host", PROPERTY_HINT_NONE), "set_host", "get_host");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "callback_method", PROPERTY_HINT_NONE), "set_callback_method", "get_callback_method");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "tag", PROPERTY_HINT_NONE), "set_tag", "get_tag");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "port", PROPERTY_HINT_NONE), "set_port", "get_port");
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

bool NetRequest::start(Ref<NetRequestData> p_data)
{
    _done = false;
    request_data = p_data;
    // print_line(String("Connecting to: ") + p_data->get_host() + itos(p_data->get_port()));
    int error = client.connect_to_host(p_data->get_host(), p_data->get_port());
    state = State::CONNECTING;
    return error == OK;
}

void NetRequest::finish_request()
{
    // print_line(String("Request finished...."));
    _done = true;
    request_response = Ref<NetRequestResponse>(memnew(NetRequestResponse));
    request_response->set_error(client.get_response_code());
    request_response->set_data(body);
}

void NetRequest::_do_connecting()
{
    bool connecting = client.get_status() == HTTPClient::Status::STATUS_CONNECTING || client.get_status() == HTTPClient::Status::STATUS_RESOLVING;

    if (connecting)
    {
        // print_line(String("Polling from connecting..."));
        Error error = client.poll();
        if (error != OK)
        {
            finish_request();
        }

        // OS::get_singleton()->delay_usec(500 * 1000);
    }
    else
    {
        state = State::REQUESTING;
        if (client.get_status() == HTTPClient::Status::STATUS_CONNECTED)
        {
            Error error = client.request_raw(
                request_data->get_method(),
                request_data->get_url_params(),
                request_data->get_headers(),
                request_data->get_body()
            );

            if (error != OK)
            {
                finish_request();
            }
        }
        else
        {
            finish_request();
        }
    }
}

void NetRequest::_do_requesting()
{
    HTTPClient::Status s = client.get_status();
    bool requesting = s == HTTPClient::Status::STATUS_REQUESTING;

    if (requesting)
    {
        // print_line(String("Polling from requesting..."));
        Error error = client.poll();
        if (error != OK)
        {
            finish_request();
        }

        // OS::get_singleton()->delay_usec(500 * 1000);
    }
    else
    {
        bool body = s == HTTPClient::STATUS_BODY || s == HTTPClient::Status::STATUS_CONNECTED;
        if (body)
        {
            state = State::FETCHING;
        }
        else
        {
            // _done = true;
            // request_response = Ref<NetRequestResponse>(memnew(NetRequestResponse));
            // request_response->set_error(client.get_response_code());
            // request_response->set_data(PoolByteArray());
            finish_request();
        }
    }
}

void NetRequest::_do_fetching()
{
    HTTPClient::Status s = client.get_status();
    bool fetching = s == HTTPClient::Status::STATUS_BODY;
    if (fetching)
    {
        // print_line(String("Polling from fetching..."));
        Error error = client.poll();
        if (error != OK)
        {
            finish_request();
            return;
        }
        PoolByteArray chunk = client.read_response_body_chunk();
        body.append_array(chunk);

        // OS::get_singleton()->delay_usec(500 * 1000);
    }
    else
    {
        // _done = true;
        // request_response = Ref<NetRequestResponse>(memnew(NetRequestResponse));
        // request_response->set_error(OK);
        // request_response->set_data(body);
        finish_request();
        
    }
}

void NetRequest::update()
{
    switch(state)
    {
        case State::CONNECTING:
            // print_line(String("---------- Do connecting"));
            _do_connecting();
        break;
        case State::REQUESTING:
            // print_line(String("---------- Do request"));
            _do_requesting();
        break;
        case State::FETCHING:
            // print_line(String("---------- Do fetching"));
            _do_fetching();
        break;
    }
}

void NetRequest::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("start", "request_data"), &NetRequest::start);
    ClassDB::bind_method(D_METHOD("update"), &NetRequest::update);
    ClassDB::bind_method(D_METHOD("is_done"), &NetRequest::is_done);
    ClassDB::bind_method(D_METHOD("get_response"), &NetRequest::get_response);

    ADD_SIGNAL(MethodInfo("request_finished", PropertyInfo(Variant::OBJECT, "response")));
}