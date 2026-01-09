#include <iostream>
#include "../include/menu.hpp"
#include "../include/database.hpp"
using namespace std;

DatabaseConnection* db = nullptr;

int main() {
    try {
        db = new DatabaseConnection("dbname=online_store user=postgres password=1234 host=localhost port=1234");
        cout << "Система запущена с PostgreSQL!" << endl;
    } catch (const exception& e) {
        cout << "Ошибка БД: " << e.what() << endl;
        return 1;
    }

    int choice = -1;
    while(true) {
        printMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число.\n";
            continue;
        }

        switch (choice) {
        case 1: menuAdmin(); 
            cout << "\nНажмите Enter для возврата...";
            cin.ignore();
            cin.get();
            break;
        case 2: menuManager(); 
            cout << "\nНажмите Enter для возврата...";
            cin.ignore();
            cin.get();
            break;
        case 3: menuUser();
            cout << "\nНажмите Enter для возврата...";
            cin.ignore();
            cin.get();
            break;
        case 4: 
            cout << "До свидания!\n";
            delete db;
            return 0;
        default:
            break;
        }
    }
    delete db;
    return 0;
}
