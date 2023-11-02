#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "core/reference.h"
#include "core/vector.h"
#include "core/os/thread.h"
#include "core/os/semaphore.h"

#include "job.h"

#ifdef JM_ENABLE_SYNC
    #define DEFINE_UPDATE_LOCKER(VARIABLE) Semaphore _jm_##VARIABLE
    #define UPDATE_CALLED(VARIABLE) _jm_##VARIABLE.post()
    // #define UNLOCK_UPDATE(VARIABLE) jm->_jm_##VARIABLE = false
    #define GET_LOCKER(VARIABLE) _jm_##VARIABLE
    #define LOOP_LOCKER(VARIABLE) jm->GET_LOCKER(VARIABLE).wait()
    #define RESET_UPDATE(VARIABLE) 
    
#else
    #define DEFINE_UPDATE_LOCKER(VARIABLE)
    #define UPDATE_CALLED(VARIABLE)
    #define RESET_UPDATE(VARIABLE) 
    #define GET_LOCKER(VARIABLE)
    #define LOOP_LOCKER(VARIABLE)
    #define RESET_UPDATE(VARIABLE)
    #define PRINT_DEBUG print_line("UNSYNC!!!!")
#endif

#define DEFINE_NO_BLOCKING_UPDATE DEFINE_UPDATE_LOCKER(_no_blocking_update)
#define NO_BLOCKING_UPDATE_CALLED UPDATE_CALLED(_no_blocking_update)
#define NO_BLOCKING_RESET_UPDATE RESET_UPDATE(_no_blocking_update)
#define NO_BLOCKING_LOOP_LOCKER LOOP_LOCKER(_no_blocking_update)

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

        PoolStringArray _job_ids;

        Mutex _search_job_mutex;

        bool is_running_no_blocking;
        bool is_running_blocking;
        

        bool has_started = false;
        bool is_cancelled = false;

        DEFINE_NO_BLOCKING_UPDATE;
    
    protected:
        static void _bind_methods();
        static void _run_no_blocking(void* user_data);
        static void _run_blocking(void* user_data);

    private:
        void dispatch_jobs(Vector<Ref<AJob>>& p_jobs);
        void update_jobs(Vector<Ref<AJob>>& p_current_jobs);
        void move_jobs(Vector<Ref<AJob>>& p_jobs, Vector<Ref<AJob>>& p_current_jobs);
        void loop_jobs(Vector<Ref<AJob>>& p_jobs, Vector<Ref<AJob>>& p_current_jobs, const Mutex& p_mutex);
    
    public:
        bool add_job(Ref<AJob> p_job);
        void cancel_job(const String& p_job);
        void cancel_no_blocking_jobs();
        void cancel_blocking_jobs();
        void cancel_all_jobs()
        {
            cancel_no_blocking_jobs();
            cancel_blocking_jobs();
        }

        PoolStringArray get_job_ids() const;

        void update();


        // void _on_job_finished(Ref<AJob> p_job, Ref<JobResult> p_result);
        // void cancel_job(const String& job_id);
        // String get_job_id(Ref<AJob> job) { return job->call("get_job_id"); } // Dummy

        bool _search_job(Ref<AJob> p_new_job);

        void start();
        void stop();
        void run_no_blocking();
        void run_blocking();
        
        JobManager();
        ~JobManager();

};

#endif