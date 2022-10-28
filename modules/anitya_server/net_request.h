#ifndef NET_REQUEST_H
#define NET_REQUEST_H

#include "core/reference.h"
#include "core/io/http_client.h"
#include "core/object.h"

class NetRequestData: public Reference
{
    GDCLASS(NetRequestData, Reference);

    private:
        Object* requester;
        HTTPClient::Method method;
        Vector<String> headers;
        PoolByteArray body;
        String url_params;
        String host; 
        int port;     

    protected:
        static void _bind_methods();

    public:
        void set_requester(Object* p_requester) { requester = p_requester; }
        Object* get_requester() const { return requester; }

        void set_method(HTTPClient::Method p_method) { method = p_method; }
        HTTPClient::Method get_method() const { return method; }

        void set_headers(const Vector<String>& p_headers) { headers = p_headers; }
        Vector<String> get_headers() const { return headers; }

        void set_body(const PoolByteArray& p_body) { body = p_body; }
        PoolByteArray get_body() const { return body; }

        void set_url_params(const String& p_url_params) { url_params = p_url_params; }
        String get_url_params() const { return url_params; }

        void set_host(const String& p_host) { host = p_host; }
        String get_host() const { return host; }

        void set_port(int p_port) { port = p_port; }
        int get_port() const { return port; }
};


class NetRequestResponse: public Reference
{
    GDCLASS(NetRequestResponse, Reference);

    private:
        int error;
        PoolByteArray data;
        String tag;

    protected:
        static void _bind_methods();

    public:
        void set_error(int p_error) { error = p_error; }
        int get_error() const { return error; }

        void set_data(const PoolByteArray& p_data) { data = p_data; }
        PoolByteArray get_data() const { return data; }

        void set_tag(const String& p_tag) { tag = p_tag; }
        String get_tag() const { return tag; }
};



class NetRequest: public Reference
{
    GDCLASS(NetRequest, Reference);

    private:
        Ref<NetRequestData> request_data;
        Ref<NetRequestResponse> request_response;
        HTTPClient client;
        bool _done = false;
        PoolByteArray body;

        enum State
        {
            CONNECTING,
            REQUESTING,
            FETCHING
        };

        State state;

    private:
        void _do_connecting();
        void _do_requesting();
        void _do_fetching();
        void finish_request();
    
    protected:
        static void _bind_methods();

    public:
        bool start(Ref<NetRequestData> request);
        void update();
        Ref<NetRequestResponse> get_response() { return request_response; }
        bool is_done() const { return _done; }
};

#endif