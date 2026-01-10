#pragma once
#include <iostream>
using namespace std;

class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() = default;
};

class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Карта: -" << amount << "₽\n";
    }
};

class CashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Наличные: -" << amount << "₽\n";
    }
};

class LoyaltyPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Бонусы: -" << amount*0.1 << "₽ (скидка 10%)\n";
    }
};