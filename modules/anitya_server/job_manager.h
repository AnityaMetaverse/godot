#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "core/reference.h"
#include "core/vector.h"
#include "core/os/thread.h"

#include "job.h"

class JobManager: public Reference
{
    GDCLASS(JobManager, Reference);

    private:
        Vector<Ref<AJob>> hig_jobs;
        Vector<Ref<AJob>> normal_jobs;
        Vector<Ref<AJob>> low_jobs;
        Vector<Ref<AJob>> current_jobs;
        Vector<Ref<AJob>> real_time_job;
        Thread job_thread;
        Mutex job_mutex;

        bool has_started = false;
        bool is_cancelled = false;
    
    protected:
        static void _bind_methods();
        static void _run(void* user_data);
    
    public:
        bool add_job(Ref<AJob> p_job);
        void _on_job_finished(Ref<AJob> p_job, Ref<JobResult> p_result);
        // void cancel_job(const String& job_id);
        String get_job_id(Ref<AJob> job) { return job->call("get_job_id"); } // Dummy

        void start();
        void stop();
        void run();
        
        JobManager();
        ~JobManager();

};

#endif