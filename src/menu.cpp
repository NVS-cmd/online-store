#include <iostream>
#include "../include/menu.hpp"
using namespace std;

extern DatabaseConnection* db;

void printMenu() {
    cout << "\n===== Пожалуйста, выберете свою роль: =====\n";
    cout << "1. Войти как Администратор\n";
    cout << "2. Войти как Менеджер\n";
    cout << "3. Войти как Покупатель\n";
    cout << "4. Выход\n";
}

void menuAdmin() {
    int choice;
    cout << "\n\033[1m===== Меню администратора: =====\033[0m\n";
    cout << "1.  Добавить новый продукт\n";
    cout << "2.  Обновить информацию о продукте\n";
    cout << "3.  Удалить продукт\n";
    cout << "4.  Просмотр всех заказов\n";
    cout << "5.  Просмотр деталей заказа\n";
    cout << "6.  Изменить статус заказа\n";
    cout << "7.  Просмотр истории статусов заказа\n";
    cout << "8.  Просмотр журнала аудита\n";
    cout << "9.  Сформировать отчет (CSV)\n";
    cout << "10. Выход\n";
    cout << ">> ";
    cin >> choice;

    switch (choice)
    {
    //Добавить новый продукт
    case 1: {
        string name, price, stock;
        cout << "Введите название: "; cin >> name;
        cout << "Цена: "; cin >> price;
        cout << "На складе: "; cin >> stock;
        string sql = "INSERT INTO products(name, price, stock_quantity) VALUES('" + name + "', " + price + ", " + stock + ")";
        db->executeNonQuery(sql);
        cout << "Продукт добавлен!\n";
        }
        break;
    
    //Обновить информацию о продукте
    case 2: {
        string id, name, price;
        cout << "ID продукта: "; cin >> id;
        cout << "Новое название: "; cin >> name;
        cout << "Новая цена: "; cin >> price;
        string sql = "UPDATE products SET name='" + name + "', price='" + price + "' WHERE product_id=" + id;
        db->executeNonQuery(sql);
        cout << "Продукт обновлен!\n";
        }
        break;
    
    //Удалить продукт
    case 3: {
        string id;
        cout << "ID продукта: "; cin >> id;
        string sql = "DELETE FROM products WHERE product_id=" + id;
        db->executeNonQuery(sql);
        cout << "Продукт удален!\n";
        }
        break;

    default:
        break;
    }
}

void menuManager() {
    cout << "\n\033[1m===== Меню менеджера: =====\033[0m\n";
    cout << "1.  Просмотр заказов в ожидании утверждения\n";
    cout << "2.  Утвердить заказ\n";
    cout << "3.  Обновить количество товара на складе\n";
    cout << "4.  Просмотр деталей заказа\n";
    cout << "5.  Изменить статус заказа (в рамках полномочий)\n";
    cout << "6.  Просмотр истории утверждённых заказов\n";
    cout << "7.  Просмотр истории статусов заказов\n";
    cout << "8. Выход\n";
}

void menuUser() {
    cout << "\n\033[1m===== Меню покупателя: =====\033[0m\n";
    cout << "1.  Создать новый заказ\n";
    cout << "2.  Добавить товар в заказ\n";
    cout << "3.  Удалить товар из заказа\n";
    cout << "4.  Просмотр моих заказов\n";
    cout << "5.  Просмотр статуса заказа\n";
    cout << "6.  Оплатить заказ\n";
    cout << "7.  Оформить возврат заказа\n";
    cout << "8.  Просмотр истории статусов заказа\n";
    cout << "9. Выход\n";
}