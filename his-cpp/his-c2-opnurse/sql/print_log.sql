-- print_log table: print execution history
CREATE TABLE IF NOT EXISTS `print_log` (
    `id` varchar(32) NOT NULL COMMENT 'Primary key',
    `print_job_id` varchar(32) NOT NULL COMMENT 'Associated print_job id',
    `doc_type` varchar(32) DEFAULT NULL COMMENT 'Document type',
    `patient_name` varchar(64) DEFAULT NULL COMMENT 'Patient name',
    `print_time` datetime DEFAULT NULL COMMENT 'Print execution time',
    `printer_name` varchar(128) DEFAULT NULL COMMENT 'Printer device name',
    `is_reprint` int DEFAULT 0 COMMENT '0=first print, 1=reprint',
    `status` int DEFAULT 1 COMMENT 'Print result: 0=fail, 1=success',
    `operator` varchar(64) DEFAULT NULL COMMENT 'Operator who triggered print',
    `remark` varchar(256) DEFAULT NULL COMMENT 'Fail reason or extra info',
    `create_time` datetime DEFAULT NULL COMMENT 'Record create time',
    PRIMARY KEY (`id`),
    INDEX `idx_print_job_id` (`print_job_id`),
    INDEX `idx_print_time` (`print_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Print execution log';
