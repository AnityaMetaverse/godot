#ifndef ANITYA_ASSET_H
#define ANITYA_ASSET_H

#include "core/reference.h"


class IAsset
{
    public:
        virtual void use_data(const PoolByteArray& p_data) = 0;
};

#endif