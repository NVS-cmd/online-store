#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "database.hpp"  // Обязательно!
using namespace std;

class User {
protected:
    int user_id;
    DatabaseConnection* db;  // protected — наследники видят!

public:
    User(int id, DatabaseConnection* database) : user_id(id), db(database) {}
    
    // Чисто виртуальные
    virtual void createOrder() = 0;
    virtual void viewOrderStatus() = 0;
    virtual void cancelOrder() = 0;
    
    // Виртуальные с реализацией
    virtual void viewAllOrders() { cout << "Нет доступа\n"; }
    virtual void addProduct() { cout << "Нет доступа\n"; }
    virtual void updateOrderStatus() { cout << "Нет доступа\n"; }
    
    virtual ~User() = default;
};
