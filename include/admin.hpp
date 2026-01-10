#pragma once
#include "user.hpp"
#include <iostream>
#include <string>
using namespace std;

class Admin : public User {
public:
    Admin(int id, DatabaseConnection* database) : User(id, database) {}
    
    void createOrder() override { cout << "Admin не создает заказы\n"; }
    void viewOrderStatus() override { cout << "Admin видит все\n"; }
    void cancelOrder() override { cout << "Admin отменяет\n"; }
    
    void addProduct() override {
        string name, price, stock;
        cout << "Название: "; cin >> name;
        cout << "Цена: "; cin >> price;
        cout << "Склад: "; cin >> stock;
        this->db->executeNonQuery("INSERT INTO products(name, price, stock_quantity) VALUES('"
                                 + name + "', " + price + ", " + stock + ")");
        cout << "Продукт добавлен!\n";
    }
    
    void viewAllOrders() override {
        cout << "\n=== Все заказы ===\n";
        auto orders = this->db->executeQuery(
            "SELECT order_id||' - '||status||' ('||total_price||'₽)' FROM orders ORDER BY order_id DESC"
        );
        for (const auto& order : orders) {
            cout << order << endl;
        }
    }
    
    void updateOrderStatus() override {
        string order_id, new_status;
        cout << "ID заказа: "; cin >> order_id;
        cout << "Статус: "; cin >> new_status;
        this->db->executeNonQuery("UPDATE orders SET status='" + new_status + "' WHERE order_id=" + order_id);
        cout << "Статус обновлен!\n";
    }
};
