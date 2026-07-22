-- print_job table: unified print job for all document types
CREATE TABLE IF NOT EXISTS `print_job` (
    `id` varchar(32) NOT NULL COMMENT 'Primary key',
    `doc_type` varchar(32) NOT NULL COMMENT 'Document type: treatment/prescription/charge_receipt/reg_receipt/infusion_label/daily_settlement/exam_request/lab_request',
    `patient_name` varchar(64) DEFAULT NULL COMMENT 'Patient name',
    `treatment_item` varchar(128) DEFAULT NULL COMMENT 'Treatment item',
    `exec_dept` varchar(64) DEFAULT NULL COMMENT 'Execution department',
    `doctor_name` varchar(64) DEFAULT NULL COMMENT 'Doctor name',
    `order_time` datetime DEFAULT NULL COMMENT 'Order time',
    `exec_time` datetime DEFAULT NULL COMMENT 'Execution time',
    `executor` varchar(64) DEFAULT NULL COMMENT 'Executor',
    `status` int DEFAULT 0 COMMENT 'Print status: 0=pending 1=printed 2=reprint',
    `print_count` int DEFAULT 0 COMMENT 'Print count',
    `remark` varchar(256) DEFAULT NULL COMMENT 'Remark',
    `create_by` varchar(64) DEFAULT NULL COMMENT 'Creator',
    `create_time` datetime DEFAULT NULL COMMENT 'Create time',
    `update_by` varchar(64) DEFAULT NULL COMMENT 'Updater',
    `update_time` datetime DEFAULT NULL COMMENT 'Update time',
    PRIMARY KEY (`id`),
    INDEX `idx_doc_type` (`doc_type`),
    INDEX `idx_order_time` (`order_time`),
    INDEX `idx_create_time` (`create_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Unified print job table';

-- Test data
INSERT INTO `print_job` VALUES ('pj001', 'treatment', '张三', '青霉素皮试', '急诊科', '王医生', '2026-07-20 08:00:00', NULL, NULL, 0, 0, '青霉素过敏史', 'admin', '2026-07-20 08:00:00', NULL, NULL);
INSERT INTO `print_job` VALUES ('pj002', 'prescription', '李四', '阿莫西林胶囊', '内科', '赵医生', '2026-07-20 09:00:00', NULL, NULL, 0, 0, 'tid', 'admin', '2026-07-20 09:00:00', NULL, NULL);
INSERT INTO `print_job` VALUES ('pj003', 'lab_request', '王五', '血常规', '检验科', '孙医生', '2026-07-20 10:00:00', NULL, NULL, 0, 0, '空腹采血', 'admin', '2026-07-20 10:00:00', NULL, NULL);
