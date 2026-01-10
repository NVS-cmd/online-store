#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "user.hpp"
using namespace std;

class Manager : public User {
public:
    Manager(int id, DatabaseConnection* database) : User(id, database) {}
    
    // Обязательные виртуальные методы
    void createOrder() override { cout << "Manager не создает заказы\n"; }
    void viewOrderStatus() override { cout << "Manager видит статусы pending\n"; }
    
    // ✅ ГЛАВНОЕ МЕНЮ МЕНЕДЖЕРА (все вызовы внутри!)
    void managerMenu() {
        int choice;
        while (true) {
            system("cls");
            cout << "\n=== МЕНЮ МЕНЕДЖЕРА ===\n";
            cout << "1. Просмотр заказов в ожидании утверждения\n";
            cout << "2. Утвердить заказ\n";
            cout << "3. Обновить количество товара на складе\n";
            cout << "4. Просмотр деталей заказа\n";
            cout << "5. Изменить статус заказа\n";
            cout << "6. Просмотр истории утвержденных заказов\n";
            cout << "7. Просмотр истории статусов заказов\n";
            cout << "8. Выход\n>> ";
            cin >> choice;
            
            switch (choice) {
                case 1: viewPendingOrders(); break;
                case 2: approveOrder(); break;
                case 3: updateStock(); break;
                case 4: viewOrderDetails(); break;
                case 5: changeOrderStatus(); break;
                case 6: viewApprovedOrders(); break;
                case 7: viewStatusHistory(); break;
                case 8: return;
                default: cout << "Неверный выбор!\n";
            }
            
            cout << "\nНажмите Enter..."; 
            cin.ignore(); 
            cin.get();
        }
    }
    
    // ✅ ВСЕ МЕТОДЫ PUBLIC (никаких private!)
    void viewPendingOrders() {
        cout << "\n=== ЗАКАЗЫ В ОЖИДАНИИ ===\n";
        auto orders = db->executeQuery("SELECT order_id||' - '||total_price||' руб.' "
                                     "FROM orders WHERE status='pending'");
        if (orders.empty()) {
            cout << "  Нет заказов на утверждение\n";
            return;
        }
        for (auto& order : orders) 
            cout << "  " << order[0] << endl;
    }
    
    void approveOrder() {
        int id;
        cout << "ID заказа для утверждения: "; cin >> id;
        db->executeNonQuery("UPDATE orders SET status='approved' WHERE order_id=" + to_string(id) + " AND status='pending'");
        cout << "Заказ #" << id << " утвержден\n";
    }
    
    void updateStock() {
        int product_id; 
        string stock;
        cout << "ID товара: "; cin >> product_id;
        cout << "Новое количество на складе: "; cin >> stock;
        db->executeNonQuery("UPDATE products SET stock_quantity=" + stock + 
                           " WHERE product_id=" + to_string(product_id));
        cout << "Склад обновлен\n";
    }
    
    void viewOrderDetails() {
        int id;
        cout << "ID заказа: "; cin >> id;
        auto details = db->executeQuery(
            "SELECT p.name, oi.quantity, oi.unit_price "
            "FROM order_items oi JOIN products p ON oi.product_id = p.product_id "
            "WHERE oi.order_id = " + to_string(id));
        cout << "\n=== Детали заказа #" << id << " ===\n";
        if (details.empty()) {
            cout << "  Заказ не найден\n";
            return;
        }
        for (auto& item : details)
            cout << "  " << item[0] << " x" << item[1] << " по " << item[2] << " руб.\n";
    }
    
    void changeOrderStatus() {
        int id; string status;
        cout << "ID заказа: "; cin >> id;
        cout << "Статус (approved/shipped): "; cin >> status;
        db->executeNonQuery("UPDATE orders SET status='" + status + 
                           "' WHERE order_id=" + to_string(id));
        cout << "Статус изменен\n";
    }
    
    void viewApprovedOrders() {
        cout << "\n=== УТВЕРЖДЕННЫЕ ЗАКАЗЫ ===\n";
        auto orders = db->executeQuery("SELECT order_id||' - '||status||' ('||total_price||')' "
                                     "FROM orders WHERE status IN ('approved', 'shipped')");
        if (orders.empty()) {
            cout << "  Нет утвержденных заказов\n";
            return;
        }
        for (auto& order : orders) 
            cout << "  " << order[0] << endl;
    }
    
    void viewStatusHistory() {
        int id;
        cout << "ID заказа: "; cin >> id;
        auto history = db->executeQuery(
            "SELECT old_status, new_status, change_time "
            "FROM order_status_history WHERE order_id=" + to_string(id) + " ORDER BY change_time");
        cout << "\n=== История статусов #" << id << " ===\n";
        if (history.empty()) {
            cout << "  История пуста\n";
            return;
        }
        for (auto& change : history)
            cout << "  " << change[0] << " -> " << change[1] << " (" << change[2] << ")\n";
    }
};