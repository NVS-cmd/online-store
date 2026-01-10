#include "../include/database.hpp"
#include "../include/admin.hpp"
#include "../include/manager.hpp"
#include "../include/customer.hpp"
#include <iostream>
#include <memory>

using namespace std;

int main() {
    DatabaseConnection db("dbname=online_store user=postgres password=1234 port=1234");
    
    while (true) {
        system("cls");
        cout << "\n===== ИНТЕРНЕТ-МАГАЗИН =====\n";
        cout << "Пожалуйста, выберите свою роль:\n";
        cout << "1. Войти как Администратор\n";
        cout << "2. Войти как Менеджер\n";
        cout << "3. Войти как Покупатель\n";
        cout << "4. Выход\n>> ";
        
        int role_choice;
        cin >> role_choice;
        
        switch (role_choice) {
            case 1: {
                auto admin = std::make_unique<Admin>(1, &db);
                admin->adminMenu();     // Полное меню админа (10 пунктов)
                break;
            }
            
            case 2: {
                auto manager = std::make_unique<Manager>(2, &db);
                manager->managerMenu(); // Полное меню менеджера (8 пунктов)
                break;
            }
            
            case 3: {
                auto customer = std::make_unique<Customer>(3, &db);
                customer->customerMenu(); // Полное меню покупателя (9 пунктов)
                break;
            }
            
            case 4: 
                return 0;
                
            default: 
                cout << "Неверный выбор роли!\n";
                cout << "Нажмите Enter..."; 
                cin.ignore(); 
                cin.get();
        }
    }
    return 0;
}
