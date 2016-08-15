#include "SQLite3Worker.h"


static SQLite3Worker* sharedSQLite3Worker = nullptr;

SQLite3Worker *SQLite3Worker::getInstance() {
    if (!sharedSQLite3Worker) {
        sharedSQLite3Worker = new SQLite3Worker();
    }
    return sharedSQLite3Worker;
}

SQLite3Worker::SQLite3Worker() {
    
}

SQLite3Worker::~SQLite3Worker() {

}

bool SQLite3Worker::openDB(const char *sqlite3FilePath) {
    if (sqlite3Instance) {
        sqlite3_close(sqlite3Instance);
    }
    bool status = sqlite3_open(sqlite3FilePath, &sqlite3Instance) == SQLITE_OK ;
    if (!status) {
        sqlite3_close(sqlite3Instance);
    }
    return status;
}

// for DML
bool SQLite3Worker::execute(const char *sql) {
    char* error;
    return sqlite3_exec(sqlite3Instance, sql, NULL, NULL, &error) == SQLITE_OK;
}

// for DQL
bool SQLite3Worker::query(const char *sql, void(handler)(sqlite3_stmt *statement)) {
    sqlite3_stmt* statement;
    int status = sqlite3_prepare_v2(sqlite3Instance, sql, -1, &statement, NULL);
    if (status == SQLITE_OK) {
        if (sqlite3_step(statement) == SQLITE_DONE) {
            return true;
        }
        while (sqlite3_step(statement) == SQLITE_ROW) {
            handler(statement); // int _id = sqlite3_column_int(statement, 0); // const unsigned char* _name = sqlite3_column_text(statement, 1);
        }
    }
    sqlite3_finalize(statement);
    return false;
}