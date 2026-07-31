-- Drug-management access paths. Run once after documents/02.../zo_his_idx.sql.
CREATE INDEX idx_charge_def_instance_alive
    ON adm_charge_item_definition
       (instance_id, instance_table(64), is_deleted, price_type_code(32), lot_no(64));

CREATE INDEX idx_med_request_medication_alive
    ON med_medication_request
       (medication_id, tenant_id, is_deleted);

CREATE INDEX idx_med_dispense_medication_alive
    ON med_medication_dispense
       (medication_id, tenant_id, is_deleted);

CREATE INDEX idx_allergy_medication_alive
    ON cln_allergy_intolerance
       (medication_id, tenant_id, is_deleted);

CREATE INDEX idx_inventory_medication_alive
    ON wkf_inventory_item
       (item_id, item_table, tenant_id, is_deleted, inventory_status_enum);

CREATE INDEX idx_template_line_medication_alive
    ON cus_order_template_line
       (medication_definition_id, tenant_id, is_deleted);
