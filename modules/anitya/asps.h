#ifndef ANITYA_ENCODING_H
#define ANITYA_ENCODING_H

#include "core/reference.h"

/**
 * @brief Header struct: <magic_number><version>-<content>
 * version: ASPS version
 * content: enconde content -> co9ntent[n] = byte + 1
 * 
 */
class ASPS: public Reference
{
    GDCLASS(ASPS, Reference);
    
    
    private:
        static const uint8_t version = 1;
        static const uint32_t magic_number = 0x0A4171A0;
    
    protected:
        static void _bind_methods();
    
    public:
        int get_version() const { return version; }
        PoolByteArray encode(const PoolByteArray& p_data);
        PoolByteArray decode(const PoolByteArray& p_data);
};

#endif