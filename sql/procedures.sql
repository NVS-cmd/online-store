BEGIN;

CREATE OR REPLACE PROCEDURE create_order_with_stock_check(
    p_user_id INTEGER,
    p_product_id INTEGER,
    p_quantity INTEGER
)
LANGUAGE plpgsql AS $$
DECLARE
    v_stock INTEGER;
    v_price DECIMAL;
BEGIN
    SELECT stock_quantity, price INTO v_stock, v_price 
    FROM products WHERE product_id = p_product_id;
    
    IF v_stock >= p_quantity THEN
        UPDATE products SET stock_quantity = stock_quantity - p_quantity 
        WHERE product_id = p_product_id;
        
        INSERT INTO orders (user_id, status, total_price)
        VALUES (p_user_id, 'pending', v_price * p_quantity);
        
        COMMIT;
        RAISE NOTICE 'Заказ создан, склад обновлен';
    ELSE
        RAISE EXCEPTION 'Недостаточно товара на складе: %', v_stock;
    END IF;
END;
$$;

COMMIT;
