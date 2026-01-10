#include "../include/database.hpp"
#include <iostream>
using namespace std;

DatabaseConnection::DatabaseConnection(const string& connectionString) {
    conn = new connection(connectionString);
    cout << "PostgreSQL подключен\n";
}

vector<vector<string>> DatabaseConnection::executeQuery(const string& query) {
    vector<vector<string>> results;
    try {
        work W(*conn);
        auto res = W.exec(query);
        for (auto row : res) {
            vector<string> rowData;
            for (int i = 0; i < row.size(); i++) {
                rowData.push_back(row[i].c_str());
            }
            results.push_back(rowData);
        }
        W.commit();
        cout << "Вернул " << results.size() << " строк\n";
    } catch (const exception& e) {
        cerr << "Ошибка SELECT: " << e.what() << endl;
        results = {}; // заглушка
    }
    return results;
}

int DatabaseConnection::executeNonQuery(const string& query) {
    try {
        nontransaction N(*conn);
        auto res = N.exec(query);
        cout << "Сохранено: " << res.affected_rows() << " строк\n";
        return static_cast<int>(res.affected_rows());
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 0;
    }
}

DatabaseConnection::~DatabaseConnection() {
    delete conn;
    cout << "PostgreSQL отключен\n";
}
