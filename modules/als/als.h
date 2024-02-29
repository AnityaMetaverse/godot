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
        PoolByteArray to_audio_network_array(const PoolVector2Array& p_array, int sample_bytes_size) const;
        PoolVector2Array to_vector2_array(const PoolByteArray& p_array, int sample_bytes_size) const;
        _ALS();

};

#endif