
#include "core/reference.h"

class DummyAnitya: public Reference {
    GDCLASS(DummyAnitya, Reference);
    private:
        
    
    protected:
        static void _bind_methods();

    public:
        void print_message(DummyAnitya& obj);
};