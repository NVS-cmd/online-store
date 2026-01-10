#pragma once
#include "database.hpp"
using namespace std;

class User {
protected:
    int id;
    DatabaseConnection* db;
    
public:
    int getId() const { return id; }
    User(int userId, DatabaseConnection* d) : id(userId), db(d) {}
    
    virtual void createOrder() = 0;
    virtual void viewOrderStatus() = 0;
    virtual void viewAllOrders() { cout << "Нет доступа\n"; }
    virtual void approveOrder(int orderId) { cout << "Нет доступа\n"; }
    virtual ~User() = default;
};