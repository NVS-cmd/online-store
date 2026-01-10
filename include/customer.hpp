#pragma once
#include <iostream>
#include "user.hpp"
using namespace std;

class Customer : public User {
public:
    Customer(int id, DatabaseConnection* database) : User(id, database) {}
    
    void createOrder() override {
        this->db->executeNonQuery("INSERT INTO orders (user_id, status, total_price) VALUES (" 
                                 + to_string(this->user_id) + ", 'pending', 0.00)");
        cout << "Заказ создан!\n";
    }
    
    void viewOrderStatus() override {
        cout << "\n=== Мои заказы ===\n";
        auto orders = this->db->executeQuery(
            "SELECT order_id||' - '||status||' ('||total_price||'₽)' FROM orders WHERE user_id=" 
            + to_string(this->user_id)
        );
        for (const auto& order : orders) {
            cout << order << endl;
        }
    }
    
    void cancelOrder() override {
        string order_id;
        cout << "ID заказа: "; cin >> order_id;
        this->db->executeNonQuery("UPDATE orders SET status='canceled' WHERE order_id=" + order_id);
        cout << "Заказ отменен!\n";
    }
};
