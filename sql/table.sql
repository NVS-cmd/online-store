CREATE DATABASE online_store;
\c online_store

-- Таблица пользователей (users)
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    email VARCHAR(100),
    role VARCHAR(20) NOT NULL CHECK (role IN ('admin', 'manager', 'customer')),
    password_hash VARCHAR,
    loyalty_level INT NOT NULL CHECK (loyalty_level IN (0, 1))
);

-- Таблица продуктов (products)
CREATE TABLE products (
    product_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL CHECK (price > 0),
    stock_quantity INT NOT NULL CHECK (stock_quantity >= 0)
);

-- Таблица заказов (orders)
CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    user_id INT REFERENCES users(user_id),
    status VARCHAR(20) NOT NULL CHECK (status IN ('pending', 'completed', 'canceled', 'returned')),
    total_price DECIMAL(10,2) NOT NULL CHECK (total_price > 0),
    order_date TIMESTAMP DEFAULT NOW()
);

-- Таблица элементов заказов (order_items)
CREATE TABLE order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id INT REFERENCES orders(order_id),
    product_id INT REFERENCES products(product_id),
    quantity INT NOT NULL CHECK (quantity >= 0),
    price DECIMAL(10,2) NOT NULL CHECK (price > 0)
);

-- Таблица истории и статусов заказов (order_status_history)
CREATE TABLE order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id INT REFERENCES orders(order_id),
    old_status VARCHAR(20),
    new_status VARCHAR(20),
    changed_at TIMESTAMP DEFAULT NOW(),
    changed_by INT REFERENCES users(user_id)
);

-- Таблица логирования (audit_log)
CREATE TABLE audit_log (
    log_id SERIAL PRIMARY KEY,
    entity_type VARCHAR(20) NOT NULL CHECK (entity_type IN ('order', 'product', 'user')),
    entity_id INT,
    operation VARCHAR(20) NOT NULL CHECK (operation IN ('insert', 'update', 'delete')),
    performed_by INT,
    performed_at TIMESTAMP DEFAULT NOW()
);