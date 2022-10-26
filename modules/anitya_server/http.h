#ifndef HTTP_H
#define HTTP_H

#include "core/reference.h"
#include "core/vector.h"

#include "net_request.h"

class IHTTP: public Reference
{
    GDCLASS(IHTTP, Reference);

    public:
        virtual void make_request(Ref<NetRequestData> p_data) {}
    protected:
        static void _bind_methods();
};

#endif
