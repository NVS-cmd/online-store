CREATE OR REPLACE FUNCTION log_order_changes()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'UPDATE' AND OLD.status != NEW.status THEN
        -- В audit_log
        INSERT INTO auditlog (table_name, operation, record_id, old_status, new_status)
        VALUES ('orders', 'UPDATE', OLD.order_id, OLD.status, NEW.status);
        
        -- В order_status_history 
        INSERT INTO order_status_history (order_id, old_status, new_status, changed_at, changed_by)
        VALUES (OLD.order_id, OLD.status, NEW.status, NOW(), 1);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
