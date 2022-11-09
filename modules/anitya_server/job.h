#ifndef JOB_H
#define JOB_H

#include "core/reference.h"


class JobResult: public Reference
{
    GDCLASS(JobResult, Reference);
    private:
        bool succeed;
        Dictionary data;
    
    protected:
        static void _bind_methods();
    public:
        void set_data(const Dictionary& p_data) { data = p_data; }
        Dictionary get_data() { return data; }
        void set_succeed(bool p_succeed) { succeed = p_succeed; }
        bool get_succeed() { return succeed; }

        // void _init(bool p_succeed, const Dictionary& p_data);
};

class JobStatus: public Reference
{
    GDCLASS(JobStatus, Reference);
};

class AJob: public Reference
{
    GDCLASS(AJob, Reference);

    public:
        enum Priority
        {
            JOB_PRIORITY_HIGH,
            JOB_PRIORITY_NORMAL,
            JOB_PRIORITY_LOW,
        };

        enum Scope
        {
            JOB_SCOPE_LOCAL,
            JOB_SCOPE_REMOTE
        };

        enum Duty
        {
            JOB_DUTY_MUST,
            JOB_DUTY_MAYBE
        };

    private:
        int scope;
        int duty;
        int priority;
        String job_id;
        Ref<JobResult> result;
        Ref<JobStatus> status;
        bool _done = false;

    protected:
        static void _bind_methods();

    public:
        int get_priority() const { return priority; }
        void set_priority(int p_priority) { priority = p_priority; }
        int get_scope() const { return scope; }
        void set_scope(int p_scope) { scope = p_scope; }
        int get_duty() const { return duty; }
        void set_duty(int p_duty) { duty = p_duty; }
        Ref<JobResult> get_job_result() const { return result; }
        void set_job_result(Ref<JobResult> p_result) { result = p_result; }
        void done() { _done = true; }
        bool is_done() const { return _done; }

    public:
        virtual String get_job_id() const { return job_id; }
        virtual void start() {}
        virtual void update() {}
        AJob();
        ~AJob();
};

VARIANT_ENUM_CAST(AJob::Priority);
VARIANT_ENUM_CAST(AJob::Scope);
VARIANT_ENUM_CAST(AJob::Duty);

#endif
