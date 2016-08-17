#include "SQLite3Worker.h"

#pragma mark - SQLite3Session

SQLite3Session::SQLite3Session(sqlite3_stmt *statement) {
    _statement = statement;
}

int SQLite3Session::columnIndex(const char *columnName) {
    if (columnName) {
        int columnCount = sqlite3_column_count(_statement);
        for (int i = 0; i < columnCount; i++) {
            const char *name = sqlite3_column_name(_statement, i);
            if (strcmp(columnName, name) == 0) {
                return i;
            }
        }
    }
    
    throw "SQLite3 : Invalid column name request";
}

int SQLite3Session::getInt(const char *columnName) {
    return sqlite3_column_int(_statement, this->columnIndex(columnName));
}

double SQLite3Session::getDouble(const char *columnName) {
    return sqlite3_column_double(_statement, this->columnIndex(columnName));
}

const char *SQLite3Session::getString(const char *columnName) {
    return (const char *)sqlite3_column_text(_statement, this->columnIndex(columnName));
}

#pragma mark - SQLite3Worker

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
bool SQLite3Worker::isQueryEmpty(const char *sql) {
    bool isEmpty = true;
    SQLite3Worker::getInstance()->query(sql, [&isEmpty](sqlite3_stmt *statement){
        isEmpty = false;
        return true;
    });
    return isEmpty;
}

// void SQLite3Worker::query(const char *sql, bool(handler)(sqlite3_stmt *statement)) {}
void SQLite3Worker::query(const char *sql, std::function<bool (sqlite3_stmt *statement)> handler) {
    sqlite3_stmt* statement;
    int status = sqlite3_prepare_v2(sqlite3Instance, sql, -1, &statement, NULL);
    if (status == SQLITE_OK) {
        while (sqlite3_step(statement) == SQLITE_ROW) {
            if (handler(statement)) break; // int _id = sqlite3_column_int(statement, 0); // const unsigned char* _name = sqlite3_column_text(statement, 1);
        }
    }
    sqlite3_finalize(statement);
}

void SQLite3Worker::query(const char *sql, std::function<bool (SQLite3Session *session)> handler) {
    sqlite3_stmt* statement;
    int status = sqlite3_prepare_v2(sqlite3Instance, sql, -1, &statement, NULL);
    if (status == SQLITE_OK) {
        SQLite3Session *session = new SQLite3Session(statement);
        while (sqlite3_step(statement) == SQLITE_ROW) {
            if (handler(session)) break;
        }
        delete session;
    }
    sqlite3_finalize(statement);
}