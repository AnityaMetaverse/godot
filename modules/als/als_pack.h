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
        PoolByteArray data() const { return _data; }
        void resize(int package_size);
        void assign_data(const PoolByteArray& data) { _data = data; }
        // void parse_data(const )
        ALSPack();
};

#endif