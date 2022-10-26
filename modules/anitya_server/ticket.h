#ifndef TICKET_H
#define TICKET_H


#include "core/reference.h"
#include "job.h"

class Ticket: public Reference
{
    GDCLASS(Ticket, Reference);
    private:
        String job_id;
    protected:
        static void _bind_methods();
    
    public:
        void _init(Ref<AJob> p_job);
        String get_job_id() const { return job_id; }
};

#endif