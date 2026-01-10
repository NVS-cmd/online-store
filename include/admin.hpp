#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <iterator>
#include "user.hpp"
using namespace std;

class Admin : public User {
public:
    Admin(int id, DatabaseConnection* database) : User(id, database) {}
    
    void createOrder() override { cout << "Admin не создает\n"; }
    void viewOrderStatus() override { cout << "Admin видит все\n"; }
    
    void adminMenu() {
        int choice;
        while (true) {
            system("cls");
            cout << "\n=== МЕНЮ АДМИНИСТРАТОРА ===\n";
            cout << "1. Добавить новый продукт\n";
            cout << "2. Обновить информацию о продукте\n";
            cout << "3. Удалить продукт\n";
            cout << "4. Просмотр всех заказов\n";
            cout << "5. Просмотр деталей заказа\n";
            cout << "6. Изменить статус заказа\n";
            cout << "7. Просмотр истории статусов заказа\n";
            cout << "8. Просмотр журнала аудита\n";
            cout << "9. Сформировать отчет (CSV)\n";
            cout << "10. Выход\n>> ";
            cin >> choice;
            
            try {
                switch (choice) {
                    case 1: addProduct(); break;
                    case 2: updateProduct(); break;
                    case 3: deleteProduct(); break;
                    case 4: viewAllOrders(); break;
                    case 5: viewOrderDetails(); break;
                    case 6: changeOrderStatus(); break;
                    case 7: viewStatusHistory(); break;
                    case 8: viewAuditLog(); break;
                    case 9: generateCSVReport(); break;
                    case 10: return;
                    default: cout << "Неверный выбор!\n";
                }
            } catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
            
            cout << "\nНажмите Enter..."; 
            cin.ignore(); 
            cin.get();
        }
    }
    
public:
    void addProduct() {
        string name, price, stock;
        cout << "Название: "; cin >> name;
        cout << "Цена: "; cin >> price;
        cout << "Склад: "; cin >> stock;
        db->executeNonQuery("INSERT INTO products(name,price,stock_quantity) VALUES('"
                          + name + "'," + price + "," + stock + ")");
        cout << "Продукт добавлен\n";
    }
    
    void viewAllOrders() override {
        cout << "\n=== Все заказы ===\n";
        auto orders = db->executeQuery("SELECT order_id||' - '||status||' ('||total_price||')' FROM orders");
        for (auto& order : orders) cout << "  " << order[0] << endl;
    }
    
    void analyzeOrders() {
        auto orders = db->executeQuery("SELECT total_price FROM orders");
        vector<double> prices;
        
        transform(orders.begin(), orders.end(), back_inserter(prices),
            [](const vector<string>& row) { return stod(row[0]); });
        
        double total = accumulate(prices.begin(), prices.end(), 0.0);
        vector<double> expensive;
        copy_if(prices.begin(), prices.end(), back_inserter(expensive),
            [](double p) { return p > 10000; });
        
        cout << "\n=== АНАЛИЗ (STL) ===\n";
        cout << "Заказов: " << prices.size() << endl;
        cout << "Сумма: " << total << " руб.\n";
        cout << "Дорогих (>10k): " << expensive.size() << endl;
    }

private:
    void updateProduct() {
        int id; string name, price, stock;
        cout << "ID продукта: "; cin >> id;
        cout << "Название: "; cin >> name;
        cout << "Цена: "; cin >> price;
        cout << "Склад: "; cin >> stock;
        
        db->executeNonQuery("UPDATE products SET name='" + name + "', price=" + price + 
                           ", stock_quantity=" + stock + " WHERE product_id=" + to_string(id));
        cout << "Продукт обновлен\n";
    }
    
    void deleteProduct() {
        int id; 
        cout << "ID продукта: "; cin >> id;
        db->executeNonQuery("DELETE FROM products WHERE product_id=" + to_string(id));
        cout << "Продукт удален\n";
    }
    
    void viewOrderDetails() {
        int id; 
        cout << "ID заказа: "; cin >> id;
        auto details = db->executeQuery(
            "SELECT p.name, oi.quantity, oi.unit_price "
            "FROM order_items oi JOIN products p ON oi.product_id = p.product_id "
            "WHERE oi.order_id = " + to_string(id));
        cout << "\n=== Детали заказа #" << id << " ===\n";
        for (auto& item : details)
            cout << "  " << item[0] << " x" << item[1] << " по " << item[2] << " руб.\n";
    }
    
    void changeOrderStatus() {
        int id; string status;
        cout << "ID заказа: "; cin >> id;
        cout << "Статус (pending/approved/shipped): "; cin >> status;
        db->executeNonQuery("UPDATE orders SET status='" + status + 
                           "' WHERE order_id=" + to_string(id));
        cout << "Статус изменен\n";
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
    
    void viewAuditLog() {
        cout << "\n=== ЖУРНАЛ АУДИТА (последние 20) ===\n";
        auto audit = db->executeQuery(
            "SELECT operation, table_name, timestamp FROM audit_log "
            "ORDER BY timestamp DESC LIMIT 20");
        if (audit.empty()) {
            cout << "  Лог пуст\n";
            return;
        }
        for (auto& log : audit)
            cout << "  " << log[0] << " (" << log[1] << ") " << log[2] << endl;
    }
    
    void generateCSVReport() {
        auto orders = db->executeQuery("SELECT order_id, status, total_price, created_at FROM orders ORDER BY created_at DESC");
        string filename = "report_" + to_string(time(nullptr)) + ".csv";
        ofstream csv(filename);
        csv << "ID,Статус,Сумма,Дата\n";
        
        for (auto& order : orders) {
            csv << order[0] << "," << order[1] << "," << order[2] << "," << order[3] << "\n";
        }
        csv.close();
        cout << "Отчет сохранен: " << filename << endl;
    }
};
