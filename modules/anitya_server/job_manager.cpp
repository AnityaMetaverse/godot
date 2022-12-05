#include "job_manager.h"

#include "modules/godot_tracy/profiler.h"

void JobManager::start()
{
    if (has_started)
    {
        WARN_PRINT("The thread already started");
        return;
    }
    no_blocking_job_thread.start(&JobManager::_run_no_blocking, this);
    blocking_job_thread.start(&JobManager::_run_blocking, this);
    is_cancelled = false;
    has_started = true;
}

void JobManager::_run_no_blocking(void* user_data)
{
    JobManager* jm = (JobManager*)user_data;
    jm->run_no_blocking();
}

void JobManager::_run_blocking(void* user_data)
{
    JobManager* jm = (JobManager*)user_data;
    jm->run_blocking();
}

void JobManager::run_no_blocking()
{
    while(!is_cancelled)
    {
        no_blocking_job_mutex.lock();
        { // Just to be fancy.
            move_jobs(no_blocking_jobs, current_no_blocking_jobs);
        }
        no_blocking_job_mutex.unlock();
        update_jobs(current_no_blocking_jobs);
    }
}

void JobManager::run_blocking()
{
    while(!is_cancelled)
    {
        blocking_job_mutex.lock();
        { // Just to be fancy.
            // while (blocking_jobs.size() != 0)
            // {
            //     Ref<AJob> job = blocking_jobs[0];
            //     job->call("start");
            //     current_blocking_jobs.push_back(job);
            //     p_jobs.remove(0);
            // }
            move_jobs(blocking_jobs, current_blocking_jobs);
        }
        blocking_job_mutex.unlock();
        update_jobs(current_blocking_jobs);
    }
}

void JobManager::move_jobs(Vector<Ref<AJob>>& p_jobs, Vector<Ref<AJob>>& p_current_jobs)
{
    while (p_jobs.size() != 0)
    {
        Ref<AJob> job = p_jobs[0];
        job->call("start");
        p_current_jobs.push_back(job);
        p_jobs.remove(0);
    }
}

void JobManager::stop()
{
    is_cancelled = true;
    blocking_job_thread.wait_to_finish();
    no_blocking_job_thread.wait_to_finish();
}

bool JobManager::add_job(Ref<AJob> p_job)
{
    if (!has_started)
    {
        WARN_PRINT("JobManager wasn't initialized");
        return false;
    }

    // Error err = p_job->connect("finished", this, "_on_job_finished");
    // if (err != OK)
    // {
    //     WARN_PRINT(String("Error connecting signal") + String(itos(err)));
    // }

    switch(p_job->get_scope())
    {
        case AJob::Scope::JOB_SCOPE_REMOTE:
            no_blocking_job_mutex.lock();
                print_line("Adding remote job");
                no_blocking_jobs.push_back(p_job);
            no_blocking_job_mutex.unlock();
            break;
        
        case AJob::Scope::JOB_SCOPE_LOCAL:
            blocking_job_mutex.lock();
                print_line("Adding local job");
                blocking_jobs.push_back(p_job);
            blocking_job_mutex.unlock();
            break;
    }





    // job_mutex.lock();

    
    // current_jobs.push_back(p_job);
    
    // job_mutex.unlock();
    return true;
}

void JobManager::update_jobs(Vector<Ref<AJob>>& p_current_jobs)
{
    ZoneScopedN( "Update Jobs" );

    for (int index = 0; index < p_current_jobs.size(); index++)
    {
        Ref<AJob> job = p_current_jobs.get(index);
        job->call("update");
        if (job->is_done())
        {
            Ref<JobResult> result = job->get_job_result();
            String id = job->call("get_job_id");
            // p_current_jobs.remove(index);
            emit_signal("job_finished", id, result);
        }
    }
    dispatch_jobs(p_current_jobs);
}

void JobManager::dispatch_jobs(Vector<Ref<AJob>>& p_jobs)
{
    // I don't know how efficient this is. It is just an approach to remove
    // a set of elements.

    int index = 0;
    while (index < p_jobs.size())
    {
        Ref<AJob> job = p_jobs[index];
        if (job->is_done())
        {
            p_jobs.remove(index);
            // memdelete(job.ptr());
            continue;
        }
        else
        {
            index++;
        }
    }
}

// void JobManager::_on_job_finished(Ref<AJob> p_job, Ref<JobResult> p_result)
// {
//     WARN_PRINT(String("--------------------- Removing job"));
//     job_mutex.lock();
//     {
//         current_jobs.erase(p_job);
//         emit_signal("job_finished", p_job->get_job_id(), p_result);
//     }
//     job_mutex.unlock();
// }

void JobManager::_bind_methods()
{
    // ClassDB::bind_method(D_METHOD("get_job_id"), &JobManager::get_job_id);
    ClassDB::bind_method(D_METHOD("start"), &JobManager::start);
    ClassDB::bind_method(D_METHOD("stop"), &JobManager::stop);
    ClassDB::bind_method(D_METHOD("add_job", "job"), &JobManager::add_job);
    // ClassDB::bind_method(D_METHOD("_on_job_finished", "job", "job_result"), &JobManager::_on_job_finished);
    ADD_SIGNAL(MethodInfo("job_finished", PropertyInfo(Variant::STRING, "job_id"), PropertyInfo(Variant::OBJECT, "job_result")));
}

JobManager::JobManager()
{
    // job_thread = memnew(Thread);
}

JobManager::~JobManager()
{
    stop();
    // memdelete(job_thread);
}