BEGIN;

CREATE OR REPLACE FUNCTION log_order_changes()
RETURNS TRIGGER AS $$
DECLARE
BEGIN
    IF TG_OP = 'UPDATE' THEN
        INSERT INTO auditlog (table_name, operation, record_id, old_status, new_status)
        VALUES ('orders', 'UPDATE', OLD.order_id, OLD.status, NEW.status);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER orders_audit_trigger
    AFTER UPDATE ON orders
    FOR EACH ROW EXECUTE FUNCTION log_order_changes();

COMMIT;
