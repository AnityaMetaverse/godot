#include "job_manager.h"


void JobManager::start()
{
    if (has_started)
    {
        WARN_PRINT("The thread already started");
        return;
    }
    job_thread.start(&JobManager::_run, this);
    is_cancelled = false;
    has_started = true;
}

void JobManager::_run(void* user_data)
{
    JobManager* jm = (JobManager*)user_data;
    jm->run();
}

void JobManager::run()
{
    while(!is_cancelled)
    {
        job_mutex.lock();
        { // Just to be fancy.
            for (int index = 0; index < current_jobs.size(); index++)
            {
                Ref<AJob> job = current_jobs.get(index);
                job->call("update");
                if (job->is_done())
                {
                    print_error(String("JOb is done"));
                    Ref<JobResult> result = job->get_job_result();
                    String id = job->call("get_job_id");
                    current_jobs.remove(index);
                    emit_signal("job_finished", id, result);
                }
            }
        }
        job_mutex.unlock();
    }
}

void JobManager::stop()
{
    is_cancelled = true;
}

bool JobManager::add_job(Ref<AJob> p_job)
{
    if (!has_started)
    {
        WARN_PRINT("JobManager wasn't initialized");
        return false;
    }
    job_mutex.lock();

    Error err = p_job->connect("finished", this, "_on_job_finished");
    if (err != OK)
    {
        WARN_PRINT(String("Error connecting signal") + String(itos(err)));
    }
    p_job->call("start");
    current_jobs.push_back(p_job);
    
    job_mutex.unlock();
    return true;
}

void JobManager::_on_job_finished(Ref<AJob> p_job, Ref<JobResult> p_result)
{
    WARN_PRINT(String("--------------------- Removing job"));
    job_mutex.lock();
    {
        current_jobs.erase(p_job);
        emit_signal("job_finished", p_job->get_job_id(), p_result);
    }
    job_mutex.unlock();
}

void JobManager::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_job_id"), &JobManager::get_job_id);
    ClassDB::bind_method(D_METHOD("start"), &JobManager::start);
    ClassDB::bind_method(D_METHOD("stop"), &JobManager::stop);
    ClassDB::bind_method(D_METHOD("add_job", "job"), &JobManager::add_job);
    ClassDB::bind_method(D_METHOD("_on_job_finished", "job", "job_result"), &JobManager::_on_job_finished);
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