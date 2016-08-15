#ifndef SQLite3Worker_hpp
#define SQLite3Worker_hpp

#include <stdio.h>
#include "sqlite3.h"

class SQLite3Worker
{
public:
    
    static SQLite3Worker *getInstance();
    
    SQLite3Worker();
    virtual ~SQLite3Worker();
    
    bool openDB(const char *sqlite3FilePath);
    bool execute(const char *sql);
    bool query(const char *sql, void(handler)(sqlite3_stmt *statement));
    
private:
    
    sqlite3* sqlite3Instance = nullptr;
};

#endif /* SQLite3Worker_hpp */
