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

        Thread no_blocking_job_thread;
        Mutex no_blocking_job_mutex;
        Vector<Ref<AJob>> no_blocking_jobs;
        Vector<Ref<AJob>> current_no_blocking_jobs;

        Thread blocking_job_thread;
        Mutex blocking_job_mutex;
        Vector<Ref<AJob>> blocking_jobs;
        Vector<Ref<AJob>> current_blocking_jobs;
        

        bool has_started = false;
        bool is_cancelled = false;
    
    protected:
        static void _bind_methods();
        static void _run_no_blocking(void* user_data);
        static void _run_blocking(void* user_data);
    
    public:
        bool add_job(Ref<AJob> p_job);
        // void _on_job_finished(Ref<AJob> p_job, Ref<JobResult> p_result);
        void dispatch_jobs(Vector<Ref<AJob>>& p_jobs);
        void update_jobs(Vector<Ref<AJob>>& p_jobs, Vector<Ref<AJob>>& p_current_jobs);
        // void cancel_job(const String& job_id);
        // String get_job_id(Ref<AJob> job) { return job->call("get_job_id"); } // Dummy

        void start();
        void stop();
        void run_no_blocking();
        void run_blocking();
        
        JobManager();
        ~JobManager();

};

#endif