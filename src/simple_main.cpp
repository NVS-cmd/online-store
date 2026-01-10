#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class DatabaseConnection {
public:
    void executeNonQuery(const string& sql) { cout << "✅ SQL: " << sql << endl; }
    vector<string> executeQuery(const string& sql) { 
        cout << "📊 QUERY: " << sql << endl;
        return {"1 - pending (0₽)", "2 - completed (1500₽)"}; 
    }
};

class User {
protected:
    int user_id;
    DatabaseConnection* db;
public:
    User(int id, DatabaseConnection* d) : user_id(id), db(d) {}
    virtual void createOrder() = 0;
    virtual void viewOrderStatus() = 0;
    virtual void viewAllOrders() { cout << "🚫 Нет доступа\n"; }
    virtual ~User() = default;
};

class Admin : public User {
public:
    Admin(int id, DatabaseConnection* d) : User(id, d) {}
    void createOrder() override { cout << "👑 Admin не создает\n"; }
    void viewOrderStatus() override { cout << "👑 Admin видит все\n"; }
    void viewAllOrders() override {
        cout << "\n📋 === Все заказы ===\n";
        auto orders = this->db->executeQuery("SELECT * FROM orders");
        for (auto& o : orders) cout << "  " << o << endl;
    }
    void addProduct() {
        cout << "🆕 Продукт добавлен в БД!\n";
        this->db->executeNonQuery("INSERT INTO products...");
    }
};

class Customer : public User {
public:
    Customer(int id, DatabaseConnection* d) : User(id, d) {}
    void createOrder() override {
        this->db->executeNonQuery("INSERT INTO orders VALUES (3,1,'pending',0.00)");
        cout << "✅ Заказ создан!\n";
    }
    void viewOrderStatus() override {
        cout << "\n📋 === Мои заказы ===\n";
        auto orders = this->db->executeQuery("SELECT * FROM orders WHERE user_id=1");
        for (auto& o : orders) cout << "  " << o << endl;
    }
};

int main() {
    DatabaseConnection db;
    unique_ptr<User> user;
    int choice;
    
    while (true) {
        system("cls");
        cout << "🌐 ===== ИНТЕРНЕТ-МАГАЗИН =====\n";
        cout << "1. 👑 Администратор\n";
        cout << "3. 🛒 Покупатель\n";
        cout << "4. ❌ Выход\n";
        cout << ">> "; cin >> choice;
        
        if (choice == 1) {
            user = make_unique<Admin>(1, &db);
            user->viewAllOrders();
            ((Admin*)user.get())->addProduct();
        } 
        else if (choice == 3) {
            user = make_unique<Customer>(1, &db);
            user->createOrder();
            user->viewOrderStatus();
        } 
        else if (choice == 4) break;
        
        cout << "\n⏸️  Нажмите Enter..."; cin.ignore(); cin.get();
    }
    return 0;
}
