#ifndef HTTP_H
#define HTTP_H

#include "core/reference.h"
#include "core/vector.h"

#include "net_request.h"

class IHTTP: public Reference
{
    GDCLASS(IHTTP, Reference);

    public:
        virtual bool make_request(Ref<NetRequestData> p_data) { return false; }
        virtual void update() {}
    protected:
        static void _bind_methods();
};

#endif
