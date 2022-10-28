#ifndef ANITYA_HTTP_H
#define ANITYA_HTTP_H

#include "core/reference.h"
#include "core/vector.h"
#include "core/os/mutex.h"

#include "http.h"
#include "net_request.h"

class AnityaHTTP: public IHTTP
{
    GDCLASS(AnityaHTTP, IHTTP);

    private:
        Vector<Ref<NetRequest>> requests;
        Mutex request_mutex;

        

    protected:
        static void _bind_methods();
    
    public:
        virtual bool make_request(Ref<NetRequestData> p_data) override;
        virtual void update() override;
};

#endif