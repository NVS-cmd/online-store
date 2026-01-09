#include <iostream>
#include "../include/menu.hpp"
using namespace std;

int main () {
    int choice = -1;
    while(true) {
        printMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка! Введите число.\n";
            continue;
        }

        switch (choice)
        {
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
            return 0;
        default:
            break;
        }
    }

    return 0;
}