-- ----------------------------
-- 住院退药患者队列联调造数（可选）
-- 用途：配合 his-j7-dispense 接口
--   POST /j7-dispense/api/j7/refund-medicine/page-patient-queue
-- 依赖：需已导入 zo_his_test_data.sql（复用其中的住院就诊、患者、病区、药品）
-- 可重复执行：先按固定测试 ID 删除再插入
-- ----------------------------
USE `zo_his`;

-- 关联基础数据说明：
--   周女士  patient_id=326940849401761792  encounter_id=330316881840836608  bus_no=ZY2026-06-300001  class_enum=IMP
--   小赵    patient_id=326270014617817088  encounter_id=330317605098229760  bus_no=ZY2026-06-300002  class_enum=IMP
--   病区    ward_id=322955305339588608（内科疗区）
--
-- Knife4j 建议请求体：
--   待退药（应返回 2 个患者：周女士、小赵；周女士两条明细聚成 1 行）
--   { "pageIndex": 1, "pageSize": 10, "queueTab": "PENDING" }
--   已退药（应返回周女士）
--   { "pageIndex": 1, "pageSize": 10, "queueTab": "COMPLETED" }
--   按病区
--   { "pageIndex": 1, "pageSize": 10, "queueTab": "PENDING", "wardId": "322955305339588608" }
--   按姓名
--   { "pageIndex": 1, "pageSize": 10, "queueTab": "PENDING", "keyword": "周女士" }

DELETE FROM `med_medication_dispense`
WHERE `id` IN (
  '990000000000000001',
  '990000000000000002',
  '990000000000000003',
  '990000000000000004'
);

DELETE FROM `med_medication_request`
WHERE `id` IN (
  '990000000000000101',
  '990000000000000102',
  '990000000000000103',
  '990000000000000104'
);

-- 退药申请（挂在住院就诊上）
INSERT INTO `med_medication_request` (
  `id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`,
  `is_deleted`, `version`, `bus_no`, `status_enum`, `status_changed_time`,
  `medication_id`, `patient_id`, `encounter_id`, `therapy_enum`,
  `quantity`, `unit_code`, `method_code`, `rate_code`
) VALUES
('990000000000000101', '1', NOW(6), NOW(6), '1', '1', 0, 0, 'MR-TEST-REFUND-001', 'COMPLETED', NOW(6),
 '323999010000000012', '326940849401761792', '330316881840836608', 'TEMPORARY',
 10.00, '10', 'T01', 'QD'),
('990000000000000102', '1', NOW(6), NOW(6), '1', '1', 0, 0, 'MR-TEST-REFUND-002', 'COMPLETED', NOW(6),
 '323999010000000013', '326940849401761792', '330316881840836608', 'TEMPORARY',
 5.00, '10', 'T01', 'QD'),
('990000000000000103', '1', NOW(6), NOW(6), '1', '1', 0, 0, 'MR-TEST-REFUND-003', 'COMPLETED', NOW(6),
 '322997883812974592', '326940849401761792', '330316881840836608', 'TEMPORARY',
 1.00, '8', '404', 'TID'),
('990000000000000104', '1', NOW(6), NOW(6), '1', '1', 0, 0, 'MR-TEST-REFUND-004', 'COMPLETED', NOW(6),
 '322996998399594496', '326270014617817088', '330317605098229760', 'TEMPORARY',
 2.00, '8', '404', 'TID');

-- 退药发放：
--   PENDING ×2（周女士，同就诊聚合测队列）
--   REFUNDED ×1（周女士，测 COMPLETED 页签）
--   PENDING ×1（小赵）
INSERT INTO `med_medication_dispense` (
  `id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`,
  `is_deleted`, `version`, `bus_no`, `status_enum`, `status_changed_time`, `dispense_enum`,
  `medication_id`, `patient_id`, `encounter_id`, `location_id`, `med_req_id`,
  `quantity`, `unit_code`, `dispense_quantity`, `lot_number`, `practitioner_id`
) VALUES
('990000000000000001', '1', NOW(6), NOW(6), '1', '1', 0, 0,
 'MDP-TEST-PENDING-001', 'PENDING', NOW(6), 'REFUND',
 '323999010000000012', '326940849401761792', '330316881840836608', '322954984441778176', '990000000000000101',
 10.00, '10', 10.00, 'LOT-TEST-001', '308791351298756608'),
('990000000000000002', '1', NOW(6), NOW(6), '1', '1', 0, 0,
 'MDP-TEST-PENDING-002', 'PENDING', NOW(6), 'REFUND',
 '323999010000000013', '326940849401761792', '330316881840836608', '322954984441778176', '990000000000000102',
 5.00, '10', 5.00, 'LOT-TEST-002', '308791351298756608'),
('990000000000000003', '1', NOW(6), NOW(6), '1', '1', 0, 0,
 'MDP-TEST-REFUNDED-001', 'REFUNDED', NOW(6), 'REFUND',
 '322997883812974592', '326940849401761792', '330316881840836608', '322954984441778176', '990000000000000103',
 1.00, '8', 1.00, 'LOT-TEST-003', '308791351298756608'),
('990000000000000004', '1', NOW(6), NOW(6), '1', '1', 0, 0,
 'MDP-TEST-PENDING-003', 'PENDING', NOW(6), 'REFUND',
 '322996998399594496', '326270014617817088', '330317605098229760', '322954984441778176', '990000000000000104',
 2.00, '8', 2.00, 'LOT-TEST-004', '308791351298756608');

-- 校验：与接口 PENDING 条件一致（期望 3 行明细 / 2 个患者）
SELECT e.id AS encounter_id, p.name, e.bus_no, d.status_enum, d.bus_no AS dispense_no
FROM `med_medication_dispense` d
JOIN `adm_encounter` e ON e.id = d.encounter_id AND e.is_deleted = 0
JOIN `adm_patient` p ON p.id = d.patient_id AND p.is_deleted = 0
WHERE d.is_deleted = 0
  AND UPPER(e.class_enum) IN ('INPATIENT', 'IMP')
  AND UPPER(d.dispense_enum) IN ('REFUND', 'RETURN')
  AND UPPER(d.status_enum) = 'PENDING'
  AND d.id LIKE '9900%';
