#pragma once
#include <pqxx/pqxx>
#include <vector>
#include <string>
using namespace std;
using namespace pqxx;

class DatabaseConnection {
private:
    connection* conn;
    work* txn;

public:
    DatabaseConnection(const string& connectionString);
    vector<string> executeQuery(const string& query);
    int executeNonQuery(const string& query);
    int executeNonTransaction(const string& query);
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();
    void createFunction(const string& sql);
    void createTrigger(const string& sql);
    bool getTransactionStatus();
    ~DatabaseConnection();
};
