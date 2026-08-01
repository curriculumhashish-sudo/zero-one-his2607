-- 独立审批记录表
-- 负责人：北海
-- 说明：用于记录盘点单、入库单等单据审批流转操作。

CREATE DATABASE IF NOT EXISTS `zo_his`;
USE `zo_his`;

CREATE TABLE IF NOT EXISTS `approval_record` (
  `id` VARCHAR(64) NOT NULL COMMENT '主键ID',
  `tenant_id` VARCHAR(64) DEFAULT NULL COMMENT '租户标识',
  `bus_no` VARCHAR(255) NOT NULL COMMENT '业务单号',
  `document_type_enum` VARCHAR(64) NOT NULL COMMENT '单据类型枚举，如 STOCKTAKE、IN_STOCK_ORDER',
  `operation_enum` VARCHAR(64) NOT NULL COMMENT '审批操作枚举，如 SUBMIT、APPROVE、REJECT、WITHDRAW',
  `approval_status_enum` VARCHAR(64) NOT NULL COMMENT '操作后审批状态，如 DRAFT、PENDING_APPROVAL、APPROVED、REJECTED、WITHDRAWN',
  `operator_id` VARCHAR(64) DEFAULT NULL COMMENT '操作人标识',
  `operator_name` VARCHAR(255) DEFAULT NULL COMMENT '操作人名称',
  `remark` VARCHAR(500) DEFAULT NULL COMMENT '审批意见/操作备注',
  `operation_time` DATETIME(6) NOT NULL COMMENT '操作时间',
  `created_at` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) COMMENT '创建时间',
  `updated_at` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6) COMMENT '更新时间',
  `created_by` VARCHAR(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` VARCHAR(64) DEFAULT NULL COMMENT '更新人',
  `is_deleted` TINYINT(1) NOT NULL DEFAULT 0 COMMENT '逻辑删除标记',
  `deleted_at` DATETIME(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` VARCHAR(64) DEFAULT NULL COMMENT '删除人',
  `version` INT NOT NULL DEFAULT 0 COMMENT '乐观锁版本号',
  PRIMARY KEY (`id`),
  KEY `idx_approval_record_bus_no` (`bus_no`),
  KEY `idx_approval_record_document_type` (`document_type_enum`),
  KEY `idx_approval_record_operation_time` (`operation_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='审批记录表';
