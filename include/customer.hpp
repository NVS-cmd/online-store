#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "user.hpp"
#include "payment.hpp"
using namespace std;

class Customer : public User {
public:
    Customer(int id, DatabaseConnection* database) : User(id, database), paymentMethod(nullptr) {}
    
    // Виртуальные методы - ПУБЛИЧНЫЕ
    void createOrder() override {
        cout << "Создание заказа...\n";
        int product_id, quantity;
        cout << "ID товара: "; cin >> product_id;
        cout << "Количество: "; cin >> quantity;
        
        db->executeNonQuery("INSERT INTO orders(customer_id, status, total_price) "
                           "VALUES(" + to_string(getId()) + ",'pending',0)");
        
        auto result = db->executeQuery("SELECT currval(pg_get_serial_sequence('orders','order_id'))");
        int order_id = stoi(result[0][0]);
        
        db->executeNonQuery("INSERT INTO order_items(order_id, product_id, quantity, unit_price) "
                           "SELECT " + to_string(order_id) + "," + to_string(product_id) + "," + 
                           to_string(quantity) + ", price FROM products WHERE product_id=" + to_string(product_id));
        
        cout << "Заказ #" << order_id << " создан\n";
    }
    
    void viewOrderStatus() override {
        cout << "\n=== СТАТУСЫ МОИХ ЗАКАЗОВ ===\n";
        auto orders = db->executeQuery("SELECT order_id, status FROM orders WHERE customer_id=" + to_string(getId()));
        if (orders.empty()) {
            cout << "  Нет заказов\n";
            return;
        }
        for (auto& order : orders)
            cout << "  #" << order[0] << ": " << order[1] << endl;
    }
    
    // ✅ ГЛАВНОЕ МЕНЮ ПОКУПАТЕЛЯ - ВСЕ PUBLIC
    void customerMenu() {
        int choice;
        while (true) {
            system("cls");
            cout << "\n=== МЕНЮ ПОКУПАТЕЛЯ ===\n";
            cout << "1. Создать новый заказ\n";
            cout << "2. Добавить товар в заказ\n";
            cout << "3. Удалить товар из заказа\n";
            cout << "4. Просмотр моих заказов\n";
            cout << "5. Просмотр статуса заказа\n";
            cout << "6. Оплатить заказ\n";
            cout << "7. Оформить возврат заказа\n";
            cout << "8. Просмотр истории статусов заказа\n";
            cout << "9. Выход\n>> ";
            cin >> choice;
            
            switch (choice) {
                case 1: createOrder(); break;
                case 2: addItemToOrder(); break;
                case 3: removeItemFromOrder(); break;
                case 4: viewMyOrders(); break;
                case 5: viewOrderStatus(); break;
                case 6: payOrder(); break;
                case 7: requestRefund(); break;
                case 8: viewMyStatusHistory(); break;
                case 9: return;
                default: cout << "Неверный выбор!\n";
            }
            
            cout << "\nНажмите Enter..."; 
            cin.ignore(); 
            cin.get();
        }
    }
    
    // ✅ ВСЕ МЕТОДЫ PUBLIC (без private!)
    void setPaymentMethod(unique_ptr<PaymentStrategy> method) {
        paymentMethod = move(method);
    }
    
    void addItemToOrder() {
        int order_id, product_id, qty;
        cout << "ID заказа: "; cin >> order_id;
        cout << "ID товара: "; cin >> product_id;
        cout << "Количество: "; cin >> qty;
        
        db->executeNonQuery("INSERT INTO order_items(order_id, product_id, quantity, unit_price) "
                           "SELECT " + to_string(order_id) + "," + to_string(product_id) + "," + 
                           to_string(qty) + ", price FROM products WHERE product_id=" + to_string(product_id));
        cout << "Товар добавлен\n";
    }
    
    void removeItemFromOrder() {
        int order_id, product_id;
        cout << "ID заказа: "; cin >> order_id;
        cout << "ID товара: "; cin >> product_id;
        
        db->executeNonQuery("DELETE FROM order_items WHERE order_id=" + to_string(order_id) + 
                           " AND product_id=" + to_string(product_id));
        cout << "Товар удален\n";
    }
    
    void viewMyOrders() {
        cout << "\n=== МОИ ЗАКАЗЫ ===\n";
        auto orders = db->executeQuery("SELECT order_id||' - '||status||' ('||total_price||')' "
                                     "FROM orders WHERE customer_id=" + to_string(getId()));
        if (orders.empty()) {
            cout << "  Нет заказов\n";
            return;
        }
        for (auto& order : orders) 
            cout << "  " << order[0] << endl;
    }
    
    void payOrder() {
        int order_id;
        cout << "ID заказа для оплаты: "; cin >> order_id;
        
        if (!paymentMethod) {
            cout << "Выберите способ оплаты:\n1. Карта 2. Наличные 3. Бонусы\n>> ";
            int type; cin >> type;
            if (type == 1) paymentMethod = make_unique<CardPayment>();
            else if (type == 2) paymentMethod = make_unique<CashPayment>();
            else paymentMethod = make_unique<LoyaltyPayment>();
        }
        
        auto price = db->executeQuery("SELECT total_price FROM orders WHERE order_id=" + to_string(order_id));
        if (!price.empty()) {
            paymentMethod->pay(stod(price[0][0]));
            db->executeNonQuery("UPDATE orders SET status='paid' WHERE order_id=" + to_string(order_id));
            cout << "Заказ оплачен\n";
        } else {
            cout << "Заказ не найден\n";
        }
    }
    
    void requestRefund() {
        int order_id;
        cout << "ID заказа для возврата: "; cin >> order_id;
        
        auto order = db->executeQuery("SELECT created_at, status FROM orders WHERE order_id=" + to_string(order_id) + 
                                     " AND customer_id=" + to_string(getId()));
        if (!order.empty() && order[0][1] != "shipped") {
            db->executeNonQuery("UPDATE orders SET status='refunded' WHERE order_id=" + to_string(order_id));
            cout << "Возврат оформлен\n";
        } else {
            cout << "Возврат невозможен\n";
        }
    }
    
    void viewMyStatusHistory() {
        int order_id;
        cout << "ID заказа: "; cin >> order_id;
        auto history = db->executeQuery(
            "SELECT old_status, new_status, change_time "
            "FROM order_status_history WHERE order_id=" + to_string(order_id) +
            " AND order_id IN (SELECT order_id FROM orders WHERE customer_id=" + to_string(getId()) + ") "
            "ORDER BY change_time");
        cout << "\n=== История #" << order_id << " ===\n";
        if (history.empty()) {
            cout << "  История пуста\n";
        } else {
            for (auto& change : history)
                cout << "  " << change[0] << " -> " << change[1] << " (" << change[2] << ")\n";
        }
    }

private:
    unique_ptr<PaymentStrategy> paymentMethod;
};