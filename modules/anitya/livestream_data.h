#ifndef STREAM_DATA_H
#define STREAM_DATA_H

#include <core/reference.h>

class LivestreamData: public Reference
{
    GDCLASS(LivestreamData, Reference);

    private:
        PoolByteArray _image_data;
        PoolByteArray _audio_data;

    protected:
        static void _bind_methods();
    
    public:
        PoolByteArray get_image_data() const
        {
            return _image_data;
        }

        PoolByteArray get_audio_data() const
        {
            return _audio_data;
        }

        void set_image_data(const PoolByteArray& p_data)
        {
            _image_data = p_data;
        }

        void set_audio_data(const PoolByteArray& p_data)
        {
            _audio_data = p_data;
        }

};

#endif
