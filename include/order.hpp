#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
using namespace std;

class OrderItem {
public:
    int product_id;
    int quantity;
    double price;
    
    OrderItem(int pid, int qty, double p) 
        : product_id(pid), quantity(qty), price(p) {
        cout << "OrderItem создан: " << product_id << " x" << quantity << endl;
    }
    
    double getSubtotal() const {
        return quantity * price;
    }
};

class Payment {
public:
    double amount;
    string method;  // "card", "cash"
    
    Payment(double amt, string mthd) : amount(amt), method(mthd) {
        cout << "Payment создан: " << amount << " (" << method << ")" << endl;
    }
    
    bool process() {
        cout << "Оплата " << amount << " обработана\n";
        return true;
    }
};

class Order {
private:
    int order_id;
    vector<unique_ptr<OrderItem>> items; 
    unique_ptr<Payment> payment;             
    
public:
    Order(int id) : order_id(id) {
        cout << "Order #" << order_id << " создан\n";
    }
    
    void addItem(int product_id, int quantity, double price) {
        auto item = make_unique<OrderItem>(product_id, quantity, price);
        items.push_back(move(item));
    }
    
    double getTotalPrice() const {
        double total = 0;
        for (const auto& item : items) {
            total += item->getSubtotal();
        }
        return total;
    }
    
    void makePayment(string method) {
        double total = getTotalPrice();
        payment = make_unique<Payment>(total, method);
        payment->process();
    }
    
    void print() const {
        cout << "\n=== Заказ #" << order_id << " ===" << endl;
        for (const auto& item : items) {
            cout << "Товар " << item->product_id 
                 << " x" << item->quantity 
                 << " = " << item->getSubtotal() << endl;
        }
        cout << "Итого: " << getTotalPrice() << endl;
    }
    
    ~Order() {
        cout << "Order #" << order_id << " удален (с элементами)\n";
    }
};
