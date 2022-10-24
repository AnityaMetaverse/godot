#ifndef JOB_H
#define JOB_H

#include "core/reference.h"

class AJob: public Reference
{
    GDCLASS(AJob, Reference);

    public:
        enum PRIORITY
        {
            JOB_PRIORITY_HIGH,
            JOB_PRIORITY_NORMAL,
            JOB_PRIORITY_LOW,
        };

        enum SCOPE
        {
            JOB_SCOPE_LOCAL,
            JOB_SCOPE_REMOTE
        };

        enum DUTY
        {
            JOB_DUTY_MUST,
            JOB_DUTY_MAYBE
        };

    private:
        int scope;
        int duty;
        int priority;

    protected:
        static void _bind_methods();

    public:
        int get_priority() const { return priority; }
        void set_priority(int p_priority) { priority = p_priority; }
        int get_scope() const { return scope; }
        void set_scope(int p_scope) { scope = p_scope; }
        int get_duty() const { return duty; }
        void set_duty(int p_duty) { duty = p_duty; }

    public:
        virtual void begin() {}
        virtual void update() {}
};


#endif
