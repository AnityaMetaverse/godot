#ifndef ANITYA_ASSET_H
#define ANITYA_ASSET_H

#include "core/reference.h"


class IAsset
{
    public:
        virtual void set_stream(const PoolByteArray& p_stream) = 0;
};

#endif