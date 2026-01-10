#pragma once
#include <pqxx/pqxx>
#include <string>
#include <vector>
using namespace std;
using namespace pqxx;

class DatabaseConnection {
private:
    connection* conn;
    
public:
    DatabaseConnection(const string& connectionString);
    vector<vector<string>> executeQuery(const string& query);
    int executeNonQuery(const string& query);
    ~DatabaseConnection();
};
