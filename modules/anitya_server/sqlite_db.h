#ifndef ANITYA_SQLITE_DB_H
#define ANITYA_SQLITE_DB_H


#include "modules/sqlite/sqlite.h"
#include "db.h"

class SQLiteDB: public IDB
{
    GDCLASS(SQLiteDB, IDB);

    private:
        Ref<SQLite> sqlite;
    
    protected:
        static void _bind_methods();
    
    public:
        virtual bool init(const String& p_path) override;
        bool execute(const String& p_query);
        bool execute_with_args(const String& p_query, Array args);
        Array fetch(const String& p_query);
        Array fetch_with_args(const String& p_query, Array args);
        virtual String get_error_message() const override { return sqlite->get_last_error_message(); }

    SQLiteDB();
    ~SQLiteDB();

};

#endif