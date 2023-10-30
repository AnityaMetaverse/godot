#include "job_manager.h"

#include "modules/godot_tracy/profiler.h"

void JobManager::start()
{
    if (has_started)
    {
        WARN_PRINT("The thread already started");
        return;
    }
    is_running_blocking = false;
    is_running_no_blocking = false;
    // no_blocking_job_thread.start(&JobManager::_run_no_blocking, this);
    // blocking_job_thread.start(&JobManager::_run_blocking, this);
    is_cancelled = false;
    has_started = true;
}

void JobManager::_run_no_blocking(void* user_data)
{
    JobManager* jm = (JobManager*)user_data;
    jm->loop_jobs(jm->no_blocking_jobs, jm->current_no_blocking_jobs, jm->no_blocking_job_mutex);
    jm->is_running_no_blocking = false;
    // while(!jm->is_cancelled && (jm->no_blocking_jobs.size() != 0 || jm->current_no_blocking_jobs.size() != 0))
    // {
    //     NO_BLOCKING_LOOP_LOCKER;
    //     NO_BLOCKING_RESET_UPDATE;
    //     // print_line("Looping job!");
    //     jm->no_blocking_job_mutex.lock();
    //     {
    //         jm->move_jobs(jm->no_blocking_jobs, jm->current_no_blocking_jobs);
    //     }
    //     jm->no_blocking_job_mutex.unlock();
    //     jm->update_jobs(jm->current_no_blocking_jobs);
    // }
    // jm->is_running_no_blocking = false;
    // jm->run_no_blocking();
}

void JobManager::_run_blocking(void* user_data)
{
    JobManager* jm = (JobManager*)user_data;
    jm->loop_jobs(jm->blocking_jobs, jm->current_blocking_jobs, jm->blocking_job_mutex);
    jm->is_running_blocking = false;
    // jm->run_blocking();
}

void JobManager::run_no_blocking()
{
    while(!is_cancelled)
    {
        no_blocking_job_mutex.lock();
        {
            move_jobs(no_blocking_jobs, current_no_blocking_jobs);
        }
        no_blocking_job_mutex.unlock();
        update_jobs(current_no_blocking_jobs);

        if (current_no_blocking_jobs.size() == 0)
        {
            break;
        }
    }
}

void JobManager::loop_jobs(Vector<Ref<AJob>>& p_jobs, Vector<Ref<AJob>>& p_current_jobs, const Mutex& p_mutex)
{
    while(!is_cancelled && (p_jobs.size() != 0 || p_current_jobs.size() != 0))
    {
        p_mutex.lock();
        {
            move_jobs(p_jobs, p_current_jobs);
        }
        p_mutex.unlock();
        update_jobs(p_current_jobs);
    }
}

void JobManager::run_blocking()
{
    while(!is_cancelled)
    {
        blocking_job_mutex.lock();
        {
            move_jobs(blocking_jobs, current_blocking_jobs);
        }
        blocking_job_mutex.unlock();
        update_jobs(current_blocking_jobs);
    }
}

void JobManager::update()
{
    // NO_BLOCKING_UPDATE_CALLED;

    if (!is_running_no_blocking && no_blocking_jobs.size() != 0)
    {
        // WARN_PRINT("running no blocking thread!");
        is_running_no_blocking = true;
        no_blocking_job_thread.wait_to_finish();
        no_blocking_job_thread.start(&JobManager::_run_no_blocking, this);
    }

    if (!is_running_blocking && blocking_jobs.size() != 0)
    {
        is_running_blocking = true;
        blocking_job_thread.wait_to_finish();
        blocking_job_thread.start(&JobManager::_run_blocking, this);
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
        ERR_PRINT("JobManager wasn't initialized");
        return false;
    }

    /**
     * DO not add it if it was previously added. 
     * It doesn't fail if it exists. It is just that someone is ahead of you.
     */
    if (_search_job(p_job))
    {
        return true;
    }

    switch(p_job->get_scope())
    {
        case AJob::Scope::JOB_SCOPE_REMOTE:
            no_blocking_job_mutex.lock();
            {
                no_blocking_jobs.push_back(p_job);
                _job_ids.push_back(p_job->call("get_job_id"));
            }
            no_blocking_job_mutex.unlock();
            break;
        
        case AJob::Scope::JOB_SCOPE_LOCAL:
            blocking_job_mutex.lock();
            {
                blocking_jobs.push_back(p_job);
                _job_ids.push_back(p_job->call("get_job_id"));
            }
            blocking_job_mutex.unlock();
            break;
    }
    return true;
}

void JobManager::update_jobs(Vector<Ref<AJob>>& p_current_jobs)
{
    // ZoneScopedN( "Update Jobs" );

    for (int index = 0; index < p_current_jobs.size(); index++)
    {
        Ref<AJob> job = p_current_jobs.get(index);
        job->call("update");
        if (job->is_done())
        {
            // WARN_PRINT("update_jobs: search job lock");
            _search_job_mutex.lock();
            // WARN_PRINT(itos(_job_ids.size()));
            for (int index = 0; index < _job_ids.size(); index++)
            {
                if (_job_ids[index] == job->call("get_job_id"))
                {
                    _job_ids.remove(index);
                    break;
                }
            }
            // WARN_PRINT("update_jobs: search job UNlock");
            _search_job_mutex.unlock();
            Ref<JobResult> result = job->get_job_result();
            String id = job->call("get_job_id");
            
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
            
            continue;
        }
        else
        {
            index++;
        }
    }
}

bool JobManager::_search_job(Ref<AJob> p_new_job)
{
    
    // WARN_PRINT("_search_job: Searching for a job lock");
    _search_job_mutex.lock();
    {
        // WARN_PRINT(itos(_job_ids.size()));
        for (int index = 0; index < _job_ids.size(); index++)
        {
            if (_job_ids[index] == p_new_job->call("get_job_id"))
            {
                // WARN_PRINT("_search_job: Searching for a job UNlock");
                _search_job_mutex.unlock();
                // WARN_PRINT("Job already found!!!");
                return true;
            }
        }
    }
    // WARN_PRINT("_search_job: Searching for a job UNlock");
    _search_job_mutex.unlock();

    return false;
}

void JobManager::cancel_job(const String& p_job_id)
{

}

void JobManager::cancel_blocking_jobs()
{

}

void JobManager::cancel_no_blocking_jobs()
{

}

void JobManager::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("start"), &JobManager::start);
    ClassDB::bind_method(D_METHOD("stop"), &JobManager::stop);
    ClassDB::bind_method(D_METHOD("cancel_job", "job_id"), &JobManager::cancel_job);
    ClassDB::bind_method(D_METHOD("cancel_all_jobs"), &JobManager::cancel_all_jobs);
    ClassDB::bind_method(D_METHOD("cancel_no_blocking_jobs"), &JobManager::cancel_no_blocking_jobs);
    ClassDB::bind_method(D_METHOD("cancel_blocking_jobs"), &JobManager::cancel_blocking_jobs);
    ClassDB::bind_method(D_METHOD("add_job", "job"), &JobManager::add_job);
    ClassDB::bind_method(D_METHOD("update"), &JobManager::update);
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