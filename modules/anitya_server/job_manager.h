#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "core/reference.h"
#include "core/vector.h"

#include "job.h"

class JobManager: public Reference
{
    GDCLASS(JobManager, Reference);

    private:
        Vector<Ref<AJob>> hig_jobs;
        Vector<Ref<AJob>> normal_jobs;
        Vector<Ref<AJob>> low_jobs;

        bool is_cancelled = false;
    
    public:
        bool add_job(Ref<AJob> p_job);

};

#endif