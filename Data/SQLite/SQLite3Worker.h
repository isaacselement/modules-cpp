#ifndef SQLite3Worker_hpp
#define SQLite3Worker_hpp

#include <stdio.h>
#include "sqlite3.h"

#pragma mark - SQLite3Session

class SQLite3Session {

public:
    
    SQLite3Session(sqlite3_stmt *statement);    
    int columnIndex(const char *columnName);
    
    int getInt(const char *columnName);
    double getDouble(const char *columnName);
    const char *getString(const char *columnName);
    
private:
    
    sqlite3_stmt *_statement;
};


#pragma mark - SQLite3Worker

class SQLite3Worker {
    
public:
    
    static SQLite3Worker *getInstance();
    
    SQLite3Worker();
    virtual ~SQLite3Worker();
    
    bool openDB(const char *sqlite3FilePath);
    bool isOpened();
    
    bool execute(const char *sql);
    
    bool isQueryEmpty(const char *sql);
    void query(const char *sql, std::function<bool (sqlite3_stmt *statement)> handler);
    void query(const char *sql, std::function<bool (SQLite3Session *session)> handler);
    int queryScalar(const char *sql);
    
    
private:
    
    sqlite3* sqlite3Instance = nullptr;
};

#endif /* SQLite3Worker_hpp */
