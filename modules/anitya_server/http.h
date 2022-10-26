#ifndef HTTP_H
#define HTTP_H

#include "core/reference.h"

class IHTTP: public Reference
{
    GDCLASS(IHTTP, Reference);

    public:
        virtual void make_get_request(const String& url) {}
        virtual void make_post_requesT(const String& url, PoolByteArray body) {}
        
};

#endif
