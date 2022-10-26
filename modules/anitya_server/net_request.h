#ifndef NET_REQUEST_H
#define NET_REQUEST_H

#include "core/reference.h"


class NetRequestData: public Reference
{
    GDCLASS(NetRequestData, Reference);

    private:
        Ref<Reference> requester;
        String method;
        PoolStringArray headers;
        PoolByteArray body;

    protected:
        static void _bind_methods();

    public:
        void set_requester(Ref<Reference> p_requester) { requester = p_requester; }
        Ref<Reference> get_requester() const { return requester; }

        void set_method(const String& p_method) { method = p_method; }
        String get_method() const { return method; }

        void set_headers(const PoolStringArray p_headers) { headers = p_headers; }
        PoolStringArray get_headers() const { return headers; }

        void set_body(const PoolByteArray& p_body) { body = p_body; }
        PoolByteArray get_body() const { return body; }
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
    
    protected:
        static void _bind_methods();

    public:
        void start(Ref<NetRequestData> request);
        void update();
};

#endif