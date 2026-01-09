#include "../include/database.hpp"
#include <iostream>
using namespace std;

DatabaseConnection::DatabaseConnection(const string& connectionString) {
    conn = new connection(connectionString);
    cout << "PostgreSQL подключен!" << endl;
}

vector<string> DatabaseConnection::executeQuery(const string& query) {
    vector<string> results;
    try {
        work W(*conn);
        auto res = W.exec(query);
        for (auto row : res) {
            results.push_back(row[0].c_str());
        }
        W.commit();
        cout << "Вернул " << results.size() << " строк" << endl;
    } catch (const exception& e) {
        cerr << "Ошибка SELECT: " << e.what() << endl;
    }
    return results;
}

int DatabaseConnection::executeNonQuery(const string& query) {
    try {
        nontransaction N(*conn);
        auto res = N.exec(query);
        cout << "Сохранено: " << res.affected_rows() << " строк" << endl;
        return res.affected_rows();
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return -1;
    }
}

DatabaseConnection::~DatabaseConnection() {
    delete conn;
    cout << "Отключен от PostgreSQL" << endl;
}
