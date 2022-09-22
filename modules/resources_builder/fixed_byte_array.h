
#include "core/reference.h"

class FixedByteArray: public Reference {
    GDCLASS(FixedByteArray, Reference);
    private:
        PoolByteArray data;
        int current_pos = 0;
    
    protected:
        static void _bind_methods();

    public:
        void push(const PoolByteArray& byte_data);
        PoolByteArray get_byte_data();
        void set_size(int p_size);

        FixedByteArray();
        ~FixedByteArray();
};