-- =====================================================
-- Test data for his-c2-opnurse patient module
-- Run this in order: 1-2-3-4-5-6
-- =====================================================

-- 1. adm_patient (3 patients)
INSERT INTO adm_patient (id, tenant_id, created_at, is_deleted, version, bus_no, name, gender_enum, birth_date, status_enum, active_flag, temp_flag, organization_id)
VALUES
('p001', 'tenant1', NOW(6), 0, 0, 'p001', '张三', 'M', '1990-05-20', 'active', 1, 0, 'org1'),
('p002', 'tenant1', NOW(6), 0, 0, 'p002', '李四', 'F', '1985-11-08', 'active', 1, 0, 'org1'),
('p003', 'tenant1', NOW(6), 0, 0, 'p003', '王五', 'M', '2000-01-15', 'active', 1, 0, 'org1');

-- 2. adm_encounter (3 encounters, linked to patients)
INSERT INTO adm_encounter (id, tenant_id, created_at, is_deleted, version, patient_id, bus_no, status_enum, class_enum, yb_class_text, organization_id)
VALUES
('e001', 'tenant1', NOW(6), 0, 0, 'p001', 'e001', 'active', 'OUTPATIENT', '医保', 'org1'),
('e002', 'tenant1', NOW(6), 0, 0, 'p002', 'e002', 'active', 'OUTPATIENT', '自费', 'org1'),
('e003', 'tenant1', NOW(6), 0, 0, 'p003', 'e003', 'active', 'OUTPATIENT', '医保', 'org1');

-- 3. wkf_appointment_queue (3 queue entries)
INSERT INTO wkf_appointment_queue (id, tenant_id, created_at, is_deleted, version, appointment_kind, bus_no, patient_id, encounter_id, organization_id, triage_level, display_order, start_instant, booking_date)
VALUES
('q001', 'tenant1', NOW(6), 0, 0, 'OUTPATIENT_BOOKING', 'A001', 'p001', 'e001', 'org1', '普通', 1, '2026-07-26 09:00:00', '2026-07-26'),
('q002', 'tenant1', NOW(6), 0, 0, 'OUTPATIENT_BOOKING', 'A002', 'p002', 'e002', 'org1', '普通', 2, '2026-07-26 09:30:00', '2026-07-26'),
('q003', 'tenant1', NOW(6), 0, 0, 'OUTPATIENT_BOOKING', 'A003', 'p003', 'e003', 'org1', '急诊', 3, '2026-07-26 10:00:00', '2026-07-26');

-- 4. wkf_service_request (execution items for patients)
INSERT INTO wkf_service_request (id, tenant_id, created_at, is_deleted, version, status_enum, category_enum, activity_id, quantity, unit_code, patient_id, encounter_id, requester_id, authored_time, entrust_content, perform_flag)
VALUES
('sr001', 'tenant1', NOW(6), 0, 0, 'COMPLETED', 'EXECUTE', 'act001', 1.00, '次', 'p001', 'e001', 'doc01', '2026-07-26 08:00:00', '静脉注射葡萄糖', 1),
('sr002', 'tenant1', NOW(6), 0, 0, 'PENDING', 'EXECUTE', 'act002', 2.00, '次', 'p001', 'e001', 'doc01', '2026-07-26 08:10:00', '血常规检查', 0),
('sr003', 'tenant1', NOW(6), 0, 0, 'COMPLETED', 'SKIN_TEST', 'act003', 1.00, '次', 'p002', 'e002', 'doc02', '2026-07-26 08:30:00', '青霉素皮试', 1),
('sr004', 'tenant1', NOW(6), 0, 0, 'PENDING', 'INFUSION', 'act004', 1.00, '袋', 'p002', 'e002', 'doc02', '2026-07-26 08:45:00', '0.9%氯化钠注射液 500ml', 0),
('sr005', 'tenant1', NOW(6), 0, 0, 'PENDING', 'EXECUTE', 'act005', 1.00, '次', 'p003', 'e003', 'doc03', '2026-07-26 09:00:00', '心电图检查', 0);

-- 5. adm_device_definition (consumables/devices)
INSERT INTO adm_device_definition (id, tenant_id, created_at, is_deleted, version, name, model_number, merchandise_name, total_volume, status_enum, active_flag)
VALUES
('dev001', 'tenant1', NOW(6), 0, 0, '一次性注射器', 'BD-5ml', '碧迪注射器', '100支/盒', 'active', 1),
('dev002', 'tenant1', NOW(6), 0, 0, '输液器', 'SYQ-001', '双鹰输液器', '50套/箱', 'active', 1),
('dev003', 'tenant1', NOW(6), 0, 0, '医用棉签', 'MQ-10cm', '海氏海诺棉签', '200支/包', 'active', 1);

-- 6. wkf_device_request (consumable usage for patients)
INSERT INTO wkf_device_request (id, tenant_id, created_at, is_deleted, version, status_enum, quantity, device_def_id, encounter_id, patient_id, unit_code)
VALUES
('dr001', 'tenant1', NOW(6), 0, 0, 'COMPLETED', 2.00, 'dev001', 'e001', 'p001', '支'),
('dr002', 'tenant1', NOW(6), 0, 0, 'COMPLETED', 1.00, 'dev002', 'e001', 'p001', '套'),
('dr003', 'tenant1', NOW(6), 0, 0, 'PENDING', 5.00, 'dev003', 'e002', 'p002', '支'),
('dr004', 'tenant1', NOW(6), 0, 0, 'PENDING', 1.00, 'dev001', 'e003', 'p003', '支');
