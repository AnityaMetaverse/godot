#include "sqlite_db.h"

bool SQLiteDB::init(const String& p_path)
{
    return sqlite->open(p_path);
}

bool SQLiteDB::execute(const String& p_query)
{
    return sqlite->query(p_query);
}

bool SQLiteDB::execute_with_args(const String& p_query, Array args)
{
    return sqlite->query_with_args(p_query, args);
}

Array SQLiteDB::fetch(const String& p_query)
{
    return sqlite->fetch_array(p_query);
}

Array SQLiteDB::fetch_with_args(const String& p_query, Array args)
{
    return sqlite->fetch_array_with_args(p_query, args);
}

// String SQLiteDB::get_message_error() const
// {
//     return sqlite->get_last_error_message();
// }

void SQLiteDB::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("init", "path"), &SQLiteDB::init);
    ClassDB::bind_method(D_METHOD("execute", "query"), &SQLiteDB::execute);
    ClassDB::bind_method(D_METHOD("execute_with_args", "query", "args"), &SQLiteDB::execute_with_args);
    ClassDB::bind_method(D_METHOD("fetch", "query"), &SQLiteDB::fetch);
    ClassDB::bind_method(D_METHOD("fetch_with_args", "query", "args"), &SQLiteDB::fetch_with_args);
    ClassDB::bind_method(D_METHOD("get_error_message"), &SQLiteDB::get_error_message);
}


SQLiteDB::SQLiteDB(): sqlite(memnew(SQLite))
{

}


SQLiteDB::~SQLiteDB()
{
    
}