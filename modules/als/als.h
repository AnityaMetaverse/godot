#ifndef ALS_H
#define ALS_H

#include "core/object.h"

class _ALS: public Object
{
    GDCLASS(_ALS, Object);

    private:
        static _ALS* _singleton;

    protected:
        static void _bind_methods();
    
    public:
        static _ALS* get_singleton() { return _singleton; }
        PoolByteArray compress_from_vector2_array(const PoolVector2Array& p_array);
        PoolVector2Array uncompress_to_vector2_array(const PoolByteArray& array);

        static void _compress_from_vector2_array(uint8_t* destiny, const float* source, int samount, bool surround = true);
        static void _uncompress_to_vector2_array(float* destiny, const uint8_t* source, int samount);

        // int assemble_pack(const PoolByteArray& array);
        _ALS();

};

#endif