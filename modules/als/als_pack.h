#ifndef ALS_ALS_PACK_H
#define ALS_ALS_PACK_H

#include "core/reference.h"

class ALSPack: public Reference 
{
    GDCLASS(ALSPack, Reference);

    private:
        PoolByteArray _data;
        int _sample_bytes_size = 2;

    protected:
        static void _bind_methods();

    public:
        void pack_from_vector2_array(const PoolVector2Array& p_array);
        PoolVector2Array unpack_to_vector2_array() const;
        int assemble_pack(const PoolByteArray& array);
        PoolByteArray data() const { return _data; }
};

#endif