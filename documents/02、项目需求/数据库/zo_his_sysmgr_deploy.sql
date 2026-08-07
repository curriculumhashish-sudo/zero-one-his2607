-- =====================================================================
-- 文件名 : zo_his_sysmgr_deploy.sql
-- 用途   : his-j1-sysmgr 系统管理微服务部署初始化脚本
-- 编码   : UTF-8（无 BOM）
--
-- 覆盖模块：
--   1) 菜单管理：菜单结构及系统管理基础菜单
--   2) 角色管理：角色、角色声明及基础角色数据
--   3) 权限管理：权限定义、角色授权及权限基础数据
--   4) 用户管理：用户、租户、组织、岗位和医疗资源关联数据
--
-- 安全约束：
--   1) 不使用 DROP TABLE/TRUNCATE，避免破坏共享环境已有数据
--   2) DDL 使用 IF NOT EXISTS，初始化数据可重复执行
--   3) 表结构以 documents/02、项目需求/数据库/zo_his.sql 和当前实体为准
--   4) 执行前必须确认目标 JDBC 数据库为 zo_his
-- =====================================================================

CREATE DATABASE IF NOT EXISTS `zo_his`
    DEFAULT CHARACTER SET utf8mb4
    DEFAULT COLLATE utf8mb4_general_ci;

USE `zo_his`;
SET NAMES utf8mb4;
SET @OLD_FOREIGN_KEY_CHECKS = @@FOREIGN_KEY_CHECKS;
SET FOREIGN_KEY_CHECKS = 0;

-- =====================================================================
-- 一、菜单管理
-- =====================================================================

CREATE TABLE IF NOT EXISTS `whale_menus` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `code` varchar(128) NOT NULL COMMENT '菜单编码',
  `name` varchar(256) NOT NULL COMMENT '菜单名称',
  `display_name` varchar(256) DEFAULT NULL COMMENT '显示名称',
  `icon` varchar(128) DEFAULT NULL COMMENT '图标',
  `path` varchar(512) DEFAULT NULL COMMENT '路由路径',
  `component` varchar(512) DEFAULT NULL COMMENT '前端组件路径',
  `permission_name` varchar(256) DEFAULT NULL COMMENT '关联权限名称',
  `parent_id` varchar(64) DEFAULT NULL COMMENT '父菜单 ID',
  `sort_order` int DEFAULT 0 COMMENT '排序号',
  `is_enabled` tinyint(1) NOT NULL DEFAULT 1 COMMENT '是否启用',
  `is_hidden` tinyint(1) NOT NULL DEFAULT 0 COMMENT '是否隐藏',
  `is_external` tinyint(1) NOT NULL DEFAULT 0 COMMENT '是否外链',
  `external_url` varchar(512) DEFAULT NULL COMMENT '外链 URL',
  `menu_type` varchar(32) NOT NULL DEFAULT 'menu' COMMENT '菜单类型（menu/directory）',
  `remark` varchar(512) DEFAULT NULL COMMENT '备注',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT 0 COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  `keep_alive` tinyint(1) DEFAULT NULL COMMENT '是否缓存页面',
  PRIMARY KEY (`id`),
  UNIQUE KEY `whale_menus_code_key` (`code`(64)),
  KEY `idx_whale_menus_is_enabled` (`is_enabled`),
  KEY `idx_whale_menus_menu_type` (`menu_type`),
  KEY `idx_whale_menus_parent_id` (`parent_id`),
  KEY `idx_whale_menus_tenant_id` (`tenant_id`),
  CONSTRAINT `fk_whale_menus_parent` FOREIGN KEY (`parent_id`) REFERENCES `whale_menus` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='菜单';

-- 系统管理基础菜单。重复执行时保留环境中已经存在的记录，不覆盖联调数据。
INSERT INTO `whale_menus`
    (`id`, `code`, `name`, `display_name`, `icon`, `path`, `component`, `permission_name`,
     `parent_id`, `sort_order`, `is_enabled`, `is_hidden`, `is_external`, `external_url`,
     `menu_type`, `remark`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`,
     `is_deleted`, `deleted_at`, `deleted_by`, `keep_alive`)
VALUES
    ('1000', 'system', 'System', '系统管理', 'lucide:settings', '/system', '/system/index', 'Navigation.System',
     NULL, 0, 1, 0, 0, '', 'directory', '', '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 1),
    ('1001', 'user_management', 'UserManagement', '用户管理', 'lucide:user', '/system/user', '/system/user/index', 'Navigation.UserManagement',
     '1000', 1, 1, 0, 0, '', 'menu', '', '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 1),
    ('1002', 'role_management', 'RoleManagement', '角色管理', 'lucide:shield-check', '/system/user/role', '/system/user/role/index', 'Navigation.RoleManagement',
     '1000', 2, 1, 0, 0, '', 'menu', '', '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 1),
    ('1004', 'permission_management', 'PermissionManagement', '权限管理', 'lucide:fingerprint-pattern', '/system/user/permission', '/system/user/permission/index', 'Navigation.PermissionManagement',
     '1000', 3, 1, 0, 0, '', 'menu', '', '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 1),
    ('1006', 'menu_management', 'MenuManagement', '菜单管理', 'lucide:square-menu', '/system/menu', '/system/menu/index', 'Navigation.MenuManagement',
     '1000', 5, 1, 0, 0, '', 'menu', '', '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 1)
ON DUPLICATE KEY UPDATE `id` = `whale_menus`.`id`;

-- =====================================================================
-- 二、角色管理
-- =====================================================================

CREATE TABLE IF NOT EXISTS `whale_roles` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `name` varchar(256) NOT NULL COMMENT '角色名称（唯一）',
  `display_name` varchar(256) DEFAULT NULL COMMENT '显示名称',
  `code` varchar(256) DEFAULT NULL COMMENT '角色编码',
  `description` text COMMENT '描述',
  `is_default` tinyint(1) NOT NULL DEFAULT 0 COMMENT '是否默认角色',
  `is_static` tinyint(1) NOT NULL DEFAULT 0 COMMENT '是否静态角色（不可删）',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT 0 COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  PRIMARY KEY (`id`),
  UNIQUE KEY `whale_roles_name_key` (`name`(64)),
  KEY `idx_whale_roles_code` (`code`(64)),
  KEY `idx_whale_roles_created_at` (`created_at`),
  KEY `idx_whale_roles_is_default` (`is_default`),
  KEY `idx_whale_roles_is_deleted` (`is_deleted`),
  KEY `idx_whale_roles_is_static` (`is_static`),
  KEY `idx_whale_roles_tenant_id` (`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='系统角色';

CREATE TABLE IF NOT EXISTS `whale_role_claims` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `role_id` varchar(64) NOT NULL COMMENT '角色标识',
  `claim_type` varchar(256) NOT NULL COMMENT '声明类型',
  `claim_value` varchar(512) DEFAULT NULL COMMENT '声明值',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT 0 COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_whale_rc_role_type_value` (`role_id`, `claim_type`(64), `claim_value`(64)),
  KEY `idx_whale_rc_claim_type` (`claim_type`(64)),
  KEY `idx_whale_rc_tenant_id` (`tenant_id`),
  CONSTRAINT `fk_whale_rc_role` FOREIGN KEY (`role_id`) REFERENCES `whale_roles` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='角色声明';

-- 内置角色使用稳定 ID，供 whale_user_roles、adm_pr_assign 等关联表引用。
-- admin 为特殊内置角色；其余内置角色可维护信息和授权，但所有内置角色均不可删除。
INSERT INTO `whale_roles`
    (`id`, `name`, `display_name`, `code`, `description`, `is_default`, `is_static`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('1',  'admin',              '系统管理员', 'ROLE_ADMIN',            '系统管理员，拥有全部权限', 0, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('10', 'doctor',             '医生',       'ROLE_DOCTOR',           '医生角色，负责诊疗相关工作', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('11', 'nurse',              '护士',       'ROLE_NURSE',            '护士角色，负责护理相关工作', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('12', 'pharmacist',         '药师',       'ROLE_PHARMACIST',       '药师角色，负责药房药品管理', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('13', 'warehouse_keeper',   '库管',       'ROLE_WAREHOUSE',        '库管角色，负责库存物资管理', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('14', 'accountant',         '财务',       'ROLE_ACCOUNTANT',       '财务角色，负责财务收费管理', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('15', 'medical_technician', '医技',       'ROLE_MEDICAL_TECH',     '医技人员，负责检验检查工作', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('16', 'staff_other',        '其他',       'ROLE_STAFF_OTHER',      '其他岗位人员', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('17', 'finance_approver',   '财务审批',   'ROLE_FINANCE_APPROVER', '财务审批岗位', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('323022177255297024', '信息科管理员', '信息科',   'ROLE_IT_ADMIN',     '信息科管理角色', 0, 0, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('329631932614709248', '库存管理员',   '库管扩展', 'ROLE_STOCK_ADMIN',  '库存管理扩展角色', 0, 0, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL)
ON DUPLICATE KEY UPDATE `id` = `whale_roles`.`id`;

-- 角色业务范围声明，来源于项目标准测试数据；重复执行时不覆盖现有授权调整。
INSERT INTO `whale_role_claims`
    (`id`, `role_id`, `claim_type`, `claim_value`, `tenant_id`, `created_at`, `updated_at`,
     `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('105013', '1',  'AssignmentScope', 'GLOBAL',                '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105015', '1',  'StaffScope',      'DOCTOR_VISIT_ORG',      '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105016', '1',  'StaffScope',      'WAREHOUSE_LOCATION',    '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105017', '1',  'StaffScope',      'PHARMACY_LOCATION',     '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105018', '1',  'StaffScope',      'NURSE_ORG',             '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105019', '1',  'StaffScope',      'NURSE_WARD_LOCATION',   '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105001', '10', 'AssignmentScope', 'ORG',                   '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105002', '10', 'StaffScope',      'DOCTOR_VISIT_ORG',      '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105004', '11', 'AssignmentScope', 'WARD',                  '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105006', '11', 'StaffScope',      'NURSE_WARD_LOCATION',   '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105026', '11', 'StaffScope',      'NURSE_ORG',             '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105007', '12', 'AssignmentScope', 'PHARMACY',              '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105008', '12', 'StaffScope',      'PHARMACY_LOCATION',     '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105009', '13', 'AssignmentScope', 'WAREHOUSE',             '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105010', '13', 'StaffScope',      'WAREHOUSE_LOCATION',    '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105011', '14', 'AssignmentScope', 'ORG',                   '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105023', '15', 'AssignmentScope', 'ORG',                   '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105024', '15', 'StaffScope',      'DOCTOR_VISIT_ORG',      '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('105025', '16', 'AssignmentScope', 'ORG',                   '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL)
ON DUPLICATE KEY UPDATE `id` = `whale_role_claims`.`id`;

-- =====================================================================
-- 三、权限管理
-- =====================================================================

CREATE TABLE IF NOT EXISTS `whale_permissions` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `name` varchar(256) NOT NULL COMMENT '权限名称（唯一）',
  `display_name` varchar(256) DEFAULT NULL COMMENT '显示名称',
  `parent_name` varchar(256) DEFAULT NULL COMMENT '父权限名称',
  `description` text COMMENT '描述',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  PRIMARY KEY (`id`),
  UNIQUE KEY `whale_permissions_name_key` (`name`(64)),
  KEY `idx_whale_permissions_parent_name` (`parent_name`(64)),
  KEY `idx_whale_permissions_tenant_id` (`tenant_id`),
  KEY `idx_whale_permissions_is_deleted` (`is_deleted`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='权限定义';

-- =====================================================================
-- 3.1 权限初始化数据（源自云库 2026-08-06 dump）
-- =====================================================================

-- 2.1 顶级权限分组（parent_name = NULL）
INSERT IGNORE INTO `whale_permissions`
    (`id`, `name`, `display_name`, `parent_name`, `description`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    (1, 'Navigation', '导航菜单', NULL, '由业务菜单自动生成的入口权限分组', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    ('310900000000000001', 'Booking', '排班预约', NULL, 'whale-booking 模块入口权限分组', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000016', 'ConfigManagement', '配置管理', NULL, 'HIS 配置管理入口权限分组（病历/执行地点/组套/取药科室/耗材绑定）', NULL,
     '2026-06-25 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000020', 'Finance', '财务管理', NULL, '财务域菜单入口权限分组', NULL,
     '2026-06-27 00:00:00.000000', '2026-06-27 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000017', 'PrintManagement', '打印管理', NULL, 'HIS 打印管理入口权限分组（PRT 模板/绑定/日志）', NULL,
     '2026-06-25 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000001', 'Report', '打印与报表', NULL, 'whale-print/report 模块入口权限分组', NULL,
     '2026-06-11 00:00:00.000000', '2026-06-11 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000018', 'ReportCenter', '报表中心', NULL, 'HIS 报表中心入口权限分组（门诊费用明细等）', NULL,
     '2026-06-25 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('322589916090863616', 'Identity.Users', '用户管理', NULL, NULL, NULL,
     '2026-06-09 04:17:09.426000', '2026-06-09 04:17:09.426000', NULL, NULL, 0, NULL, NULL),
    ('322589916199915520', 'Identity.Roles', '角色管理', NULL, NULL, NULL,
     '2026-06-09 04:17:09.452000', '2026-06-09 04:17:09.452000', NULL, NULL, 0, NULL, NULL),
    ('322589916267024384', 'Identity.Permissions', '权限管理', NULL, NULL, NULL,
     '2026-06-09 04:17:09.468000', '2026-06-09 04:17:09.468000', NULL, NULL, 0, NULL, NULL),
    ('322589916338327552', 'Identity.OrganizationUnits', '组织单元管理', NULL, NULL, NULL,
     '2026-06-09 04:17:09.485000', '2026-06-09 04:17:09.485000', NULL, NULL, 0, NULL, NULL),
    ('322589916443185152', 'TenantManagement.Tenants', '租户管理', NULL, NULL, NULL,
     '2026-06-09 04:17:09.510000', '2026-06-09 04:17:09.510000', NULL, NULL, 0, NULL, NULL),
    ('322589916527071232', 'SettingManagement.Settings', '设置管理', NULL, NULL, NULL,
     '2026-06-09 04:17:09.530000', '2026-06-09 04:17:09.530000', NULL, NULL, 0, NULL, NULL);

-- 2.2 页面级入口权限（parent_name = Navigation，对应菜单的 permission_name）
INSERT IGNORE INTO `whale_permissions`
    (`id`, `name`, `display_name`, `parent_name`, `description`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('330000000000000002', 'Navigation.System', '系统管理', 'Navigation', '系统管理目录入口权限（登录模块菜单引用）', NULL,
     '2026-08-06 12:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (2, 'Navigation.UserManagement', '用户管理', 'Navigation', '菜单入口: user_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (3, 'Navigation.RoleManagement', '角色管理', 'Navigation', '菜单入口: role_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (4, 'Navigation.PermissionManagement', '权限管理', 'Navigation', '菜单入口: permission_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (5, 'Navigation.TenantManagement', '租户管理', 'Navigation', '菜单入口: tenant_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (6, 'Navigation.MenuManagement', '菜单管理', 'Navigation', '菜单入口: menu_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (7, 'Navigation.SettingManagement', '设置管理', 'Navigation', '菜单入口: setting_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (8, 'Navigation.CodesystemManagement', '编码管理', 'Navigation', '菜单入口: codesystem_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (9, 'Navigation.BodystructureManagement', '部位管理', 'Navigation', '菜单入口: bodystructure_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (10, 'Navigation.WarehouseManagement', '库房药房', 'Navigation', '菜单入口: warehouse_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (11, 'Navigation.WardLocationManagement', '病区病房', 'Navigation', '菜单入口: ward_location_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (12, 'Navigation.PatientManagement', '患者管理', 'Navigation', '菜单入口: patient_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (14, 'Navigation.DeptManagement', '科室管理', 'Navigation', '菜单入口: dept_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (15, 'Navigation.HealthcareServiceManagement', '挂号类型管理', 'Navigation', '菜单入口: healthcare_service_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (16, 'Navigation.ContractManagement', '费用性质管理', 'Navigation', '菜单入口: contract_management', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (17, 'Navigation.Analytics', '工作台', 'Navigation', '菜单入口: Analytics', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (18, 'Navigation.Frequency', '频次管理', 'Navigation', '菜单入口: Frequency', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (19, 'Navigation.Supplier', '供应商管理', 'Navigation', '菜单入口: Supplier', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (20, 'Navigation.Medicine', '药品管理', 'Navigation', '菜单入口: Medicine', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (21, 'Navigation.Device', '器材管理', 'Navigation', '菜单入口: Device', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (22, 'Navigation.Diagnosistreatment', '诊疗管理', 'Navigation', '菜单入口: Diagnosistreatment', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (23, 'Navigation.Disease', '诊断管理', 'Navigation', '菜单入口: Disease', NULL,
     '2026-05-26 20:00:00.000000', '2026-06-30 14:42:06.362025', '1', '1', 0, NULL, NULL),
    (25, 'Navigation.Outpatientnursestation', '门诊护士站', 'Navigation', '菜单入口: OutpatientNurseStation', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (26, 'Navigation.Inpatientnursestation', '住院护士站', 'Navigation', '菜单入口: InpatientNurseStation', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (27, 'Navigation.Reportfinance', '财务报表', 'Navigation', '菜单入口: ReportFinance', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (28, 'Navigation.Chargeopregister', '门诊挂号', 'Navigation', '菜单入口: ChargeOpRegister', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (29, 'Navigation.Chargeopcharge', '门诊收费', 'Navigation', '菜单入口: ChargeOpCharge', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (30, 'Navigation.Chargeoprefund', '门诊退费', 'Navigation', '菜单入口: ChargeOpRefund', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (31, 'Navigation.Chargeopdayclose', '门诊日结', 'Navigation', '菜单入口: ChargeOpDayClose', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (32, 'Navigation.Chargeipregister', '住院登记', 'Navigation', '菜单入口: ChargeIpRegister', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (33, 'Navigation.Chargeipsettle', '住院结算', 'Navigation', '菜单入口: ChargeIpSettle', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (34, 'Navigation.Chargeipprepay', '预交金管理', 'Navigation', '菜单入口: ChargeIpPrepay', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (35, 'Navigation.Chargeipfee', '住院费用管理', 'Navigation', '菜单入口: ChargeIpFee', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (36, 'Navigation.Outpatientdoctorstation', '门诊医生站', 'Navigation', '菜单入口: OutpatientDoctorStation', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (37, 'Navigation.Inpatientdoctorstation', '住院医生站', 'Navigation', '菜单入口: InpatientDoctorStation', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (38, 'Navigation.Dispenseop', '门诊发药', 'Navigation', '菜单入口: DispenseOp', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (39, 'Navigation.Dispenseip', '住院发药', 'Navigation', '菜单入口: DispenseIp', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (41, 'Navigation.Whpurchaseorder', '药库订货单', 'Navigation', '菜单入口: WhPurchaseOrder', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (42, 'Navigation.Reporthome', '首页', 'Navigation', '菜单入口: ReportHome', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (43, 'Navigation.Reportdean', '院长', 'Navigation', '菜单入口: ReportDean', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (44, 'Navigation.Reportclinical', '临床报表', 'Navigation', '菜单入口: ReportClinical', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (45, 'Navigation.Whpurchaseinbound', '药库进货单', 'Navigation', '菜单入口: WhPurchaseInbound', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (46, 'Navigation.Whstockout', '药库出库单', 'Navigation', '菜单入口: WhStockOut', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (47, 'Navigation.Phstockin', '药房入库单', 'Navigation', '菜单入口: PhStockIn', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (48, 'Navigation.Invstocklist', '库存明细', 'Navigation', '菜单入口: InvStockList', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (49, 'Navigation.Medicinereturnop', '门诊退药', 'Navigation', '菜单入口: MedicineReturnOp', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (51, 'Navigation.Whstocktakeorder', '药库盘点单', 'Navigation', '菜单入口: WhStocktakeOrder', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (52, 'Navigation.Phstocktakeorder', '药房盘点单', 'Navigation', '菜单入口: PhStocktakeOrder', NULL,
     '2026-05-26 20:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (53, 'Navigation.Medicinereturnip', '住院退药', 'Navigation', '菜单入口: MedicineReturnIp', NULL,
     '2026-06-11 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (61, 'Navigation.Whreturntosupplier', '药库退货单', 'Navigation', '菜单入口: WhReturnToSupplier', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (63, 'Navigation.Whgainloss', '药库损益单', 'Navigation', '菜单入口: WhGainLoss', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (64, 'Navigation.Phagainloss', '药房损益单', 'Navigation', '菜单入口: PhaGainLoss', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (65, 'Navigation.Deptgainloss', '科室损益单', 'Navigation', '菜单入口: DeptGainLoss', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (66, 'Navigation.Whtransferout', '药库调拨出库', 'Navigation', '菜单入口: WhTransferOut', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (67, 'Navigation.Whtransferin', '药库调拨入库', 'Navigation', '菜单入口: WhTransferIn', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (68, 'Navigation.Phareturnupstream', '药房退库', 'Navigation', '菜单入口: PhaReturnUpstream', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (69, 'Navigation.Whreturnfromdownstream', '药库退库', 'Navigation', '菜单入口: WhReturnFromDownstream', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (70, 'Navigation.Deptreturn', '科室退库', 'Navigation', '菜单入口: DeptReturn', NULL,
     '2026-06-17 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (73, 'Navigation.Chargeipdayclose', '住院日结', 'Navigation', '菜单入口: ChargeIpDayClose', NULL,
     '2026-06-21 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (74, 'Navigation.Dispenserecordop', '门诊发退药记录', 'Navigation', '菜单入口: DispenseRecordOp', NULL,
     '2026-06-21 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    (75, 'Navigation.Dispenserecordip', '住院发退药记录', 'Navigation', '菜单入口: DispenseRecordIp', NULL,
     '2026-06-21 00:00:00.000000', NULL, '1', NULL, 0, NULL, NULL),
    ('310700000000000010', 'Navigation.ChsNationalCatalog', '国家目录查询', 'Navigation', '菜单入口: ChsNationalCatalog', NULL,
     '2026-06-15 12:00:00.000000', '2026-06-15 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000011', 'Navigation.ChsCatalogDownload', '目录文件下载', 'Navigation', '菜单入口: ChsCatalogDownload', NULL,
     '2026-06-15 12:00:00.000000', '2026-06-15 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000012', 'Navigation.ChsReconciliation', '对账清算', 'Navigation', '菜单入口: ChsReconciliation', NULL,
     '2026-06-15 12:00:00.000000', '2026-06-15 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000013', 'Navigation.ChsCatalogMappingWorkbench', '目录对码工作台', 'Navigation', '菜单入口: ChsCatalogMappingWorkbench', NULL,
     '2026-06-15 12:00:00.000000', '2026-06-15 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000014', 'Navigation.ChsInventoryOps', '进销存运维', 'Navigation', '菜单入口: ChsInventoryOps', NULL,
     '2026-06-15 12:00:00.000000', '2026-06-15 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000015', 'Navigation.ChsMedicalInfoQuery', '医保信息查询', 'Navigation', '菜单入口: ChsMedicalInfoQuery — 5201–5206', NULL,
     '2026-06-15 12:00:00.000000', '2026-06-15 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000019', 'Navigation.ChsPlatformDict', '平台字典(1901)', 'Navigation', '菜单入口: ChsPlatformDict — 1901 参保地医保区划字典', NULL,
     '2026-06-24 12:00:00.000000', '2026-06-24 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000021', 'Navigation.PriceAdjustment', '调价管理', 'Navigation', '菜单入口: PriceAdjustment — 批量调价单与审批', NULL,
     '2026-06-27 00:00:00.000000', '2026-06-27 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000022', 'Navigation.PriceAdjustmentDetail', '调价明细', 'Navigation', '菜单入口: PriceAdjustmentDetail — 已生效调价项目明细查询', NULL,
     '2026-06-27 00:00:00.000000', '2026-06-27 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('317000463887929349', 'Navigation.SpecimenDefinition', '样本管理', 'Navigation', '菜单入口: specimen_definition', NULL,
     '2026-06-12 08:18:12.580000', '2026-06-12 08:18:12.580000', '1', '1', 0, NULL, NULL),
    ('330000000000000001', 'Navigation.JobManagement', '定时任务', 'Navigation', '菜单入口: job_management', NULL,
     '2026-06-18 12:00:00.000000', '2026-06-18 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('880020000000000001', 'Navigation.MrmsCaseRecordList', '病案检索', 'Navigation', '菜单入口: MrmsCaseRecordList — 病案检索列表', NULL,
     '2026-06-27 12:00:00.000000', '2026-06-27 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('880020000000000002', 'Navigation.MrmsQcWorkbench', '质控工作台', 'Navigation', '菜单入口: MrmsQcWorkbench — 病案质控与归档', NULL,
     '2026-06-27 12:00:00.000000', '2026-06-27 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('880020000000000003', 'Navigation.MrmsBorrow', '借阅复印', 'Navigation', '菜单入口: MrmsBorrow — 病案借阅复印审批', NULL,
     '2026-06-27 12:00:00.000000', '2026-06-27 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('880020000000000004', 'Navigation.MrmsUpload', '4401上传', 'Navigation', '菜单入口: MrmsUpload — 医保病案首页手动上传', NULL,
     '2026-06-27 12:00:00.000000', '2026-06-27 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('880020000000000005', 'Navigation.MrmsCaseRecordHomepage', '病案首页编辑', 'Navigation', '隐藏路由: MrmsCaseRecordHomepage — 全屏首页编辑', NULL,
     '2026-06-27 12:00:00.000000', '2026-06-27 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('880020000000000006', 'Navigation.MrmsCaseRecordVolume', '卷宗预览', 'Navigation', '隐藏路由: MrmsCaseRecordVolume — 卷宗 PDF 预览', NULL,
     '2026-06-27 12:00:00.000000', '2026-06-27 12:00:00.000000', '1', '1', 0, NULL, NULL);

-- 2.3 配置管理子权限（parent_name = ConfigManagement）
INSERT IGNORE INTO `whale_permissions`
    (`id`, `name`, `display_name`, `parent_name`, `description`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    (50, 'Navigation.Emrtemplate', '病历模版', 'ConfigManagement', '菜单入口: EmrTemplate', NULL,
     '2026-05-26 20:00:00.000000', '2026-06-25 00:00:00.000000', '1', NULL, 0, NULL, NULL),
    (54, 'Navigation.ActivityDeviceBinding', '诊疗耗材绑定', 'ConfigManagement', '菜单入口: activity_device_binding', NULL,
     '2026-05-26 20:00:00.000000', '2026-06-25 00:00:00.000000', '1', NULL, 0, NULL, NULL),
    (55, 'Navigation.Executelocation', '执行地点', 'ConfigManagement', '菜单入口: ExecuteLocation', NULL,
     '2026-05-26 20:00:00.000000', '2026-06-25 00:00:00.000000', '1', NULL, 0, NULL, NULL),
    (56, 'Navigation.OrderTemplate', '组套模板', 'ConfigManagement', '菜单入口: order_template', NULL,
     '2026-05-26 20:00:00.000000', '2026-06-25 00:00:00.000000', '1', NULL, 0, NULL, NULL),
    (57, 'Navigation.ServiceLocation', '取药科室', 'ConfigManagement', '菜单入口: service_location', NULL,
     '2026-05-26 20:00:00.000000', '2026-06-25 00:00:00.000000', '1', NULL, 0, NULL, NULL);

-- 2.4 报表中心子权限（parent_name = ReportCenter）
INSERT IGNORE INTO `whale_permissions`
    (`id`, `name`, `display_name`, `parent_name`, `description`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    (76, 'Navigation.Opfeedetail', '门诊费用明细', 'ReportCenter', '菜单入口: OpFeeDetail', NULL,
     '2026-06-21 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', NULL, 0, NULL, NULL);

-- 2.5 打印管理子权限（parent_name = PrintManagement）
INSERT IGNORE INTO `whale_permissions`
    (`id`, `name`, `display_name`, `parent_name`, `description`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('310700000000000002', 'Print.Templates.View', '打印模板', 'PrintManagement', '菜单入口: PrintTemplate', NULL,
     '2026-06-11 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000003', 'Print.Templates.Design', '打印设计器', 'PrintManagement', '独立 hipaint 画布设计器菜单；与 PrintBinding / 业务发印无关', NULL,
     '2026-06-11 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000004', 'Print.PrintLog.View', '打印日志', 'PrintManagement', '菜单入口: ReportPrintLog', NULL,
     '2026-06-11 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000007', 'Print.PrintBinding.View', '打印模板配置', 'PrintManagement', '菜单入口: ReportPrintBinding — bindingCode→busNo', NULL,
     '2026-06-11 12:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL);

-- 2.6 报表模块子权限（parent_name = Report）
INSERT IGNORE INTO `whale_permissions`
    (`id`, `name`, `display_name`, `parent_name`, `description`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('310700000000000005', 'Report.Center.View', '报表中心', 'Report', '菜单入口: ReportCenter — 已发布 C 类报表查询/导出/打印', NULL,
     '2026-06-11 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000006', 'Health.ExternalDocument.View', '外系统报告', 'Report', '菜单入口: ExternalDocument', NULL,
     '2026-06-11 00:00:00.000000', '2026-06-11 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000008', 'Report.Designer.View', '报表设计器', 'Report', '菜单入口: ReportDesigner — SQL/表头/表底/筛选器设计', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 12:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310700000000000009', 'Report.Definition.View', '报表模板', 'Report', '菜单入口: ReportTemplate — C 类报表 CRUD', NULL,
     '2026-06-13 12:00:00.000000', '2026-06-13 12:00:00.000000', '1', '1', 0, NULL, NULL);

-- 2.7 排班预约子权限（parent_name = Booking）
INSERT IGNORE INTO `whale_permissions`
    (`id`, `name`, `display_name`, `parent_name`, `description`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('310900000000000002', 'Booking.OutpatientSchedule.View', '门诊排班', 'Booking', '菜单入口: BookingOutpatientSchedule', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000003', 'Booking.Register.View', '门诊预约', 'Booking', '门诊预约登记', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000004', 'Booking.Queue.View', '候诊叫号', 'Booking', '候诊叫号台', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000005', 'Booking.Policy.View', '预约策略', 'Booking', '放号策略与院点挂号策略', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000006', 'Booking.MedTech.View', '医技预约', 'Booking', '医技预约登记', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000007', 'Booking.Bed.View', '床位预约', 'Booking', '床位预留日历', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000008', 'Booking.HealthCheckup.View', '体检预约', 'Booking', '团检与个检预约', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000009', 'Booking.Channel.View', '渠道配置', 'Booking', '预约渠道与配额', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000010', 'Booking.QueueDisplay.View', '叫号大屏', 'Booking', '候诊叫号大屏', NULL,
     '2026-06-13 00:00:00.000000', '2026-06-13 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000011', 'Booking.Template.View', '排班设置', 'Booking', '日排班、周排班、放号时段与日历例外配置', NULL,
     '2026-06-14 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL),
    ('310900000000000012', 'Booking.SlotCalendar.View', '号源日历', 'Booking', '菜单入口: BookingSlotCalendar', NULL,
     '2026-06-25 00:00:00.000000', '2026-06-25 00:00:00.000000', '1', '1', 0, NULL, NULL);

-- 2.8 操作级权限（Identity / TenantManagement / SettingManagement 的 CRUD 子权限）
INSERT IGNORE INTO `whale_permissions`
    (`id`, `name`, `display_name`, `parent_name`, `description`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    -- Identity.Users 子权限
    ('322589916120223744', 'Identity.Users.Create', '创建用户', 'Identity.Users', NULL, NULL,
     '2026-06-09 04:17:09.433000', '2026-06-09 04:17:09.433000', NULL, NULL, 0, NULL, NULL),
    ('322589916137000960', 'Identity.Users.Update', '更新用户', 'Identity.Users', NULL, NULL,
     '2026-06-09 04:17:09.437000', '2026-06-09 04:17:09.437000', NULL, NULL, 0, NULL, NULL),
    ('322589916157972480', 'Identity.Users.Delete', '删除用户', 'Identity.Users', NULL, NULL,
     '2026-06-09 04:17:09.442000', '2026-06-09 04:17:09.442000', NULL, NULL, 0, NULL, NULL),
    ('322589916174749696', 'Identity.Users.View', '查看用户', 'Identity.Users', NULL, NULL,
     '2026-06-09 04:17:09.446000', '2026-06-09 04:17:09.446000', NULL, NULL, 0, NULL, NULL),
    ('322589916430602240', 'Identity.Users.Impersonate', '代理登录', 'Identity.Users', NULL, NULL,
     '2026-06-09 04:17:09.507000', '2026-06-09 04:17:09.507000', NULL, NULL, 0, NULL, NULL),
    -- Identity.Roles 子权限
    ('322589916216692736', 'Identity.Roles.Create', '创建角色', 'Identity.Roles', NULL, NULL,
     '2026-06-09 04:17:09.456000', '2026-06-09 04:17:09.456000', NULL, NULL, 0, NULL, NULL),
    ('322589916229275648', 'Identity.Roles.Update', '更新角色', 'Identity.Roles', NULL, NULL,
     '2026-06-09 04:17:09.459000', '2026-06-09 04:17:09.459000', NULL, NULL, 0, NULL, NULL),
    ('322589916246052864', 'Identity.Roles.Delete', '删除角色', 'Identity.Roles', NULL, NULL,
     '2026-06-09 04:17:09.463000', '2026-06-09 04:17:09.463000', NULL, NULL, 0, NULL, NULL),
    ('322589916254441472', 'Identity.Roles.View', '查看角色', 'Identity.Roles', NULL, NULL,
     '2026-06-09 04:17:09.465000', '2026-06-09 04:17:09.465000', NULL, NULL, 0, NULL, NULL),
    -- Identity.Permissions 子权限
    ('322589916287995904', 'Identity.Permissions.Grant', '授予权限', 'Identity.Permissions', NULL, NULL,
     '2026-06-09 04:17:09.473000', '2026-06-09 04:17:09.473000', NULL, NULL, 0, NULL, NULL),
    ('322589916304773120', 'Identity.Permissions.Revoke', '撤销权限', 'Identity.Permissions', NULL, NULL,
     '2026-06-09 04:17:09.477000', '2026-06-09 04:17:09.477000', NULL, NULL, 0, NULL, NULL),
    ('322589916325744640', 'Identity.Permissions.View', '查看权限', 'Identity.Permissions', NULL, NULL,
     '2026-06-09 04:17:09.482000', '2026-06-09 04:17:09.482000', NULL, NULL, 0, NULL, NULL),
    -- Identity.OrganizationUnits 子权限
    ('322589916355104768', 'Identity.OrganizationUnits.Create', '创建组织单元', 'Identity.OrganizationUnits', NULL, NULL,
     '2026-06-09 04:17:09.489000', '2026-06-09 04:17:09.489000', NULL, NULL, 0, NULL, NULL),
    ('322589916380270592', 'Identity.OrganizationUnits.Update', '更新组织单元', 'Identity.OrganizationUnits', NULL, NULL,
     '2026-06-09 04:17:09.495000', '2026-06-09 04:17:09.495000', NULL, NULL, 0, NULL, NULL),
    ('322589916397047808', 'Identity.OrganizationUnits.Delete', '删除组织单元', 'Identity.OrganizationUnits', NULL, NULL,
     '2026-06-09 04:17:09.499000', '2026-06-09 04:17:09.499000', NULL, NULL, 0, NULL, NULL),
    ('322589916413825024', 'Identity.OrganizationUnits.View', '查看组织单元', 'Identity.OrganizationUnits', NULL, NULL,
     '2026-06-09 04:17:09.503000', '2026-06-09 04:17:09.503000', NULL, NULL, 0, NULL, NULL),
    -- TenantManagement.Tenants 子权限
    ('322589916459962368', 'TenantManagement.Tenants.Create', '创建租户', 'TenantManagement.Tenants', NULL, NULL,
     '2026-06-09 04:17:09.514000', '2026-06-09 04:17:09.514000', NULL, NULL, 0, NULL, NULL),
    ('322589916476739584', 'TenantManagement.Tenants.Update', '更新租户', 'TenantManagement.Tenants', NULL, NULL,
     '2026-06-09 04:17:09.518000', '2026-06-09 04:17:09.518000', NULL, NULL, 0, NULL, NULL),
    ('322589916497711104', 'TenantManagement.Tenants.Delete', '删除租户', 'TenantManagement.Tenants', NULL, NULL,
     '2026-06-09 04:17:09.523000', '2026-06-09 04:17:09.523000', NULL, NULL, 0, NULL, NULL),
    ('322589916510294016', 'TenantManagement.Tenants.View', '查看租户', 'TenantManagement.Tenants', NULL, NULL,
     '2026-06-09 04:17:09.526000', '2026-06-09 04:17:09.526000', NULL, NULL, 0, NULL, NULL),
    -- SettingManagement.Settings 子权限
    ('322589916548042752', 'SettingManagement.Settings.Create', '创建设置', 'SettingManagement.Settings', NULL, NULL,
     '2026-06-09 04:17:09.535000', '2026-06-09 04:17:09.535000', NULL, NULL, 0, NULL, NULL),
    ('322589916564819968', 'SettingManagement.Settings.Update', '更新设置', 'SettingManagement.Settings', NULL, NULL,
     '2026-06-09 04:17:09.539000', '2026-06-09 04:17:09.539000', NULL, NULL, 0, NULL, NULL),
    ('322589916577402880', 'SettingManagement.Settings.Delete', '删除设置', 'SettingManagement.Settings', NULL, NULL,
     '2026-06-09 04:17:09.542000', '2026-06-09 04:17:09.542000', NULL, NULL, 0, NULL, NULL),
    ('322589916594180096', 'SettingManagement.Settings.View', '查看设置', 'SettingManagement.Settings', NULL, NULL,
     '2026-06-09 04:17:09.546000', '2026-06-09 04:17:09.546000', NULL, NULL, 0, NULL, NULL);

-- =====================================================================
-- 四、角色权限授权
-- =====================================================================

CREATE TABLE IF NOT EXISTS `whale_permission_grants` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `permission_name` varchar(256) NOT NULL COMMENT '权限名称',
  `provider_name` varchar(64) NOT NULL COMMENT '授权主体类型（R=角色/U=用户）',
  `provider_key` varchar(256) NOT NULL COMMENT '授权主体键',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT 0 COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_whale_pg_subject_permission` (`provider_name`, `provider_key`(64), `permission_name`(64), `tenant_id`),
  KEY `idx_whale_pg_permission_name` (`permission_name`(64)),
  KEY `idx_whale_pg_provider` (`provider_name`, `provider_key`(64)),
  KEY `idx_whale_pg_tenant_id` (`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='权限授权';

-- admin 是特殊内置角色，初始化时授予全部有效权限。使用稳定 ID，并兼容已存在但未建唯一索引的环境。
INSERT INTO `whale_permission_grants`
    (`id`, `permission_name`, `provider_name`, `provider_key`, `tenant_id`, `created_at`, `updated_at`,
     `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
SELECT CONCAT('sysmgr-admin-', p.`id`), p.`name`, 'R', '1', '1', NOW(6), NOW(6),
       '1', '1', 0, NULL, NULL
FROM `whale_permissions` p
WHERE p.`is_deleted` = 0
  AND NOT EXISTS (
      SELECT 1
      FROM `whale_permission_grants` pg
      WHERE pg.`permission_name` = p.`name`
        AND pg.`provider_name` = 'R'
        AND pg.`provider_key` = '1'
        AND pg.`tenant_id` = '1'
        AND pg.`is_deleted` = 0
  );

-- =====================================================================

-- =====================================================================
-- 五、用户管理
-- =====================================================================

-- 5.1 用户管理核心表

-- 1.1 组织机构单元表（科室树）
CREATE TABLE IF NOT EXISTS `whale_organization_units` (
    `id` varchar(64) NOT NULL COMMENT '主键',
    `code` varchar(128) NOT NULL COMMENT '编码',
    `display_name` varchar(256) NOT NULL COMMENT '显示名称',
    `parent_id` varchar(64) DEFAULT NULL COMMENT '父节点标识',
    `sort_order` int DEFAULT '0' COMMENT '排序号',
    `is_enabled` tinyint(1) NOT NULL DEFAULT '1' COMMENT '是否启用',
    `remark` varchar(512) DEFAULT NULL COMMENT '备注',
    `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
    `created_at` datetime(6) NOT NULL COMMENT '创建时间',
    `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
    `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
    `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
    `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
    `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
    `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
    PRIMARY KEY (`id`),
    UNIQUE KEY `whale_organization_units_code_key` (`code`(64)),
    KEY `idx_whale_ou_parent_id` (`parent_id`),
    KEY `idx_whale_ou_tenant_id` (`tenant_id`),
    CONSTRAINT `fk_whale_ou_parent` FOREIGN KEY (`parent_id`) REFERENCES `whale_organization_units` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='组织机构单元';

-- 1.3 管理域机构表（医院/科室）
CREATE TABLE IF NOT EXISTS `adm_organization` (
    `id` varchar(64) NOT NULL COMMENT '主键',
    `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
    `created_at` datetime(6) NOT NULL COMMENT '创建时间',
    `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
    `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
    `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
    `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
    `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
    `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
    `version` int NOT NULL DEFAULT '0' COMMENT '乐观锁版本号',
    `name` varchar(255) NOT NULL COMMENT '机构名称',
    `bus_no` varchar(255) NOT NULL COMMENT 'bus 编号',
    `parent_id` varchar(64) DEFAULT NULL COMMENT 'parent 标识',
    `type_enum` varchar(255) NOT NULL COMMENT 'type 枚举值（HOSPITAL/DEPARTMENT）',
    `class_enum` varchar(255) DEFAULT NULL COMMENT 'class 枚举值',
    `status_enum` varchar(255) NOT NULL COMMENT 'status 枚举值（ACTIVE/INACTIVE）',
    `active_flag` tinyint(1) NOT NULL COMMENT 'active 标志位',
    `display_order` int DEFAULT NULL COMMENT '显示顺序',
    `def_doctor_id` varchar(64) DEFAULT NULL COMMENT 'defDoctor 标识',
    `py_str` varchar(255) DEFAULT NULL COMMENT '拼音码',
    `wb_str` varchar(255) DEFAULT NULL COMMENT '五笔码',
    `yb_no` varchar(255) DEFAULT NULL COMMENT 'yb 编号',
    `yb_name` varchar(255) DEFAULT NULL COMMENT 'yb 名称',
    `medins_id` varchar(255) DEFAULT NULL COMMENT 'medins 标识',
    `medins_admdvs` varchar(255) DEFAULT NULL COMMENT '医疗机构医保区划',
    `medins_type` varchar(255) DEFAULT NULL COMMENT 'medins类型',
    `medins_lv` varchar(255) DEFAULT NULL COMMENT '医疗机构等级',
    `caty` varchar(255) DEFAULT NULL COMMENT '科别',
    PRIMARY KEY (`id`),
    KEY `idx_adm_org_tenant_type_status` (`tenant_id`, `type_enum`, `status_enum`, `is_deleted`, `active_flag`),
    KEY `idx_adm_org_tenant_order` (`tenant_id`, `display_order`, `name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='管理域 机构';

-- 1.4 系统用户表
CREATE TABLE IF NOT EXISTS `whale_users` (
    `id` varchar(64) NOT NULL COMMENT '主键',
    `user_name` varchar(256) NOT NULL COMMENT '登录用户名',
    `email` varchar(256) DEFAULT NULL COMMENT '邮箱',
    `phone_number` varchar(32) DEFAULT NULL COMMENT '手机号',
    `password_hash` varchar(256) NOT NULL COMMENT '密码哈希',
    `name` varchar(64) DEFAULT NULL COMMENT '名',
    `surname` varchar(64) DEFAULT NULL COMMENT '姓',
    `is_active` tinyint(1) NOT NULL DEFAULT '1' COMMENT '是否启用',
    `is_locked_out` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否锁定',
    `lockout_end` datetime(6) DEFAULT NULL COMMENT '锁定结束时间',
    `access_failed_count` int NOT NULL DEFAULT '0' COMMENT '登录失败次数',
    `extra_properties` json DEFAULT NULL COMMENT '扩展属性 JSON',
    `created_at` datetime(6) NOT NULL COMMENT '创建时间',
    `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
    `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
    `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
    `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
    `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
    `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
    `ukey_id` varchar(128) DEFAULT NULL COMMENT 'UKey 标识',
    `ukey_src_id` varchar(128) DEFAULT NULL COMMENT 'UKey 来源标识',
    PRIMARY KEY (`id`),
    UNIQUE KEY `idx_users_user_name_unique` (`user_name`(64)),
    UNIQUE KEY `idx_users_email_unique` (`email`(64)),
    KEY `idx_users_email` (`email`(64)),
    KEY `idx_users_ukey_id` (`ukey_id`(64)),
    KEY `idx_users_ukey_src_id` (`ukey_src_id`(64)),
    KEY `idx_whale_users_created_at` (`created_at`),
    KEY `idx_whale_users_is_active` (`is_active`),
    KEY `idx_whale_users_is_deleted` (`is_deleted`),
    KEY `idx_whale_users_is_locked_out` (`is_locked_out`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='系统用户';

-- 1.5 用户-角色关联表
CREATE TABLE IF NOT EXISTS `whale_user_roles` (
    `id` varchar(64) NOT NULL COMMENT '主键',
    `user_id` varchar(64) NOT NULL COMMENT '用户标识',
    `role_id` varchar(64) NOT NULL COMMENT '角色标识',
    `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
    `created_at` datetime(6) NOT NULL COMMENT '创建时间',
    `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
    `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
    `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
    `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
    `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
    `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
    PRIMARY KEY (`id`),
    UNIQUE KEY `whale_user_roles_user_id_role_id_key` (`user_id`,`role_id`),
    KEY `idx_whale_user_roles_is_deleted` (`is_deleted`),
    KEY `idx_whale_user_roles_role_id` (`role_id`),
    KEY `idx_whale_user_roles_tenant_id` (`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='用户角色关联';

-- 1.6 用户-组织单元关联表（用户可选科室）
CREATE TABLE IF NOT EXISTS `whale_user_organization_units` (
    `id` varchar(64) NOT NULL COMMENT '主键',
    `user_id` varchar(64) NOT NULL COMMENT '用户标识',
    `organization_unit_id` varchar(64) NOT NULL COMMENT '组织单元标识',
    `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
    `created_at` datetime(6) NOT NULL COMMENT '创建时间',
    `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
    `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
    `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
    `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
    `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
    `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
    PRIMARY KEY (`id`),
    UNIQUE KEY `uk_whale_uou_user_ou` (`user_id`,`organization_unit_id`),
    KEY `idx_whale_uou_ou_id` (`organization_unit_id`),
    KEY `idx_whale_uou_tenant_id` (`tenant_id`),
    CONSTRAINT `fk_whale_uou_ou` FOREIGN KEY (`organization_unit_id`) REFERENCES `whale_organization_units` (`id`),
    CONSTRAINT `fk_whale_uou_user` FOREIGN KEY (`user_id`) REFERENCES `whale_users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='用户组织单元关联';

-- 1.7 医务人员主数据表
CREATE TABLE IF NOT EXISTS `adm_practitioner` (
    `id` varchar(64) NOT NULL COMMENT '主键',
    `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
    `created_at` datetime(6) NOT NULL COMMENT '创建时间',
    `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
    `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
    `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
    `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
    `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
    `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
    `version` bigint NOT NULL DEFAULT '0' COMMENT '乐观锁版本号',
    `name` varchar(255) NOT NULL COMMENT '人员姓名',
    `name_json` varchar(2000) DEFAULT NULL COMMENT '其他名称 JSON（曾用名、英文名等）',
    `py_str` varchar(255) DEFAULT NULL COMMENT '姓名拼音检索码',
    `wb_str` varchar(255) DEFAULT NULL COMMENT '姓名五笔检索码',
    `gender_enum` varchar(255) NOT NULL COMMENT '行政性别编码 UNKNOWN(0)未知 FEMALE(1)女性 MALE(2)男性',
    `birth_date` date DEFAULT NULL COMMENT '出生日期',
    `deceased_date` datetime(6) DEFAULT NULL COMMENT '死亡时间',
    `phone` varchar(50) DEFAULT NULL COMMENT '联系电话',
    `address_province` varchar(64) DEFAULT NULL COMMENT '地址-省',
    `address_city` varchar(64) DEFAULT NULL COMMENT '地址-市',
    `address_district` varchar(64) DEFAULT NULL COMMENT '地址-区/县',
    `address_street` varchar(255) DEFAULT NULL COMMENT '地址-街道/详细地址',
    `address_json` varchar(2000) DEFAULT NULL COMMENT '地址扩展 JSON',
    `status_enum` varchar(255) NOT NULL COMMENT '人员状态（ACTIVE / INACTIVE / DISCARDED）',
    `active_flag` tinyint(1) NOT NULL DEFAULT '1' COMMENT '是否启用（status_enum == ACTIVE 时为 true）',
    `bus_no` varchar(64) DEFAULT NULL COMMENT '院内工号/业务编号',
    `yb_no` varchar(64) DEFAULT NULL COMMENT '医保人员编码',
    `user_id` varchar(64) NOT NULL COMMENT '关联平台用户主键（与 id 独立，无库表外键）',
    `org_id` varchar(64) DEFAULT NULL COMMENT '默认登录科室/当前工作科室',
    `dr_profttl_code` int DEFAULT NULL COMMENT '职称编码',
    `phar_prac_cert_no` varchar(64) DEFAULT NULL COMMENT '医师/药师执业资格证编号',
    `prsc_dr_cert_code` varchar(64) DEFAULT NULL COMMENT '执业证件类型编码',
    `signature` text COMMENT '医生签名图片 Base64（可含 data:image/*;base64, 前缀）',
    `kpd_code` varchar(64) DEFAULT NULL COMMENT '开票点编码',
    `pos_no` varchar(64) DEFAULT NULL COMMENT 'POS 机终端编号',
    PRIMARY KEY (`id`),
    KEY `idx_adm_practitioner_user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='医务人员主数据';

-- 1.8 管理域医疗服务表（可接诊号源）
CREATE TABLE IF NOT EXISTS `adm_healthcare_service` (
    `id` varchar(64) NOT NULL COMMENT '主键',
    `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
    `created_at` datetime(6) NOT NULL COMMENT '创建时间',
    `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
    `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
    `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
    `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
    `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
    `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
    `version` int NOT NULL DEFAULT '0' COMMENT '乐观锁版本号',
    `name` varchar(255) NOT NULL COMMENT '医疗服务名称',
    `type_code` varchar(255) NOT NULL COMMENT '服务类型编码（区分普通门诊、专家门诊等服务类型）',
    `category_code` varchar(255) DEFAULT NULL COMMENT '服务类别编码（区分门诊、住院等服务大类）',
    `offered_org_id` varchar(64) DEFAULT NULL COMMENT '提供该服务的机构主键',
    `specialty_code` varchar(255) DEFAULT NULL COMMENT '专科码（如 CARDIOLOGY/SURGERY）',
    `location_id` varchar(64) DEFAULT NULL COMMENT '服务所在地点主键',
    `status_enum` varchar(255) NOT NULL COMMENT '服务状态码',
    `active_flag` tinyint(1) NOT NULL COMMENT '服务是否处于激活可用状态',
    `appointment_required_flag` tinyint(1) DEFAULT NULL COMMENT '预约是否必填标志',
    `yb_no` varchar(255) DEFAULT NULL COMMENT '医保编号',
    `bus_no` varchar(255) DEFAULT NULL COMMENT '业务编号',
    `comment` varchar(255) DEFAULT NULL COMMENT '备注说明',
    `extra_details` varchar(255) DEFAULT NULL COMMENT '扩展详情（JSON 字符串）',
    `contact` varchar(255) DEFAULT NULL COMMENT '联系方式（JSON 字符串）',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='管理域 医疗服务';

-- 1.9 管理域地点表（病区/药房/库房）
CREATE TABLE IF NOT EXISTS `adm_location` (
    `id` varchar(64) NOT NULL COMMENT '主键',
    `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
    `created_at` datetime(6) NOT NULL COMMENT '创建时间',
    `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
    `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
    `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
    `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
    `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
    `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
    `version` int NOT NULL DEFAULT '0' COMMENT '乐观锁版本号',
    `name` varchar(255) NOT NULL COMMENT '位置名称',
    `bus_no` varchar(255) NOT NULL COMMENT 'bus 编号',
    `status_enum` varchar(255) NOT NULL COMMENT 'status 枚举值',
    `operational_enum` varchar(255) DEFAULT NULL COMMENT 'operational 枚举值',
    `form_enum` varchar(255) NOT NULL COMMENT 'form 枚举值（WARD=病区 / PHARMACY=药房 / WAREHOUSE=库房）',
    `mode_enum` varchar(255) NOT NULL COMMENT 'mode 枚举值',
    `type_json` varchar(255) DEFAULT NULL COMMENT 'type JSON 快照',
    `organization_id` varchar(64) NOT NULL COMMENT '机构 标识',
    `display_order` int DEFAULT NULL COMMENT '显示顺序',
    `py_str` varchar(255) DEFAULT NULL COMMENT '拼音码',
    `wb_str` varchar(255) DEFAULT NULL COMMENT '五笔码',
    `parent_id` varchar(64) DEFAULT NULL COMMENT 'parent 标识',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='管理域 地点';

-- =====================================================================
-- 5.2 用户管理补充关联表
-- =====================================================================

CREATE TABLE IF NOT EXISTS `whale_user_tenants` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `user_id` varchar(64) NOT NULL COMMENT '用户标识',
  `tenant_id` varchar(64) NOT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT 0 COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_whale_ut_user_tenant` (`user_id`, `tenant_id`),
  KEY `idx_whale_ut_tenant_id` (`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='用户租户关联';

CREATE TABLE IF NOT EXISTS `adm_pr_assign` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT 0 COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  `version` int NOT NULL DEFAULT 0 COMMENT '乐观锁版本号',
  `name` varchar(255) DEFAULT NULL COMMENT '名称',
  `active_flag` tinyint(1) NOT NULL DEFAULT 1 COMMENT '是否启用',
  `practitioner_id` varchar(64) NOT NULL COMMENT '医护人员标识',
  `org_id` varchar(64) DEFAULT NULL COMMENT '机构标识',
  `location_id` varchar(64) DEFAULT NULL COMMENT '位置/科室标识',
  `specialty_code` varchar(64) DEFAULT NULL COMMENT '专科编码',
  `type_code` int DEFAULT NULL COMMENT '类型编码',
  `role_id` varchar(64) NOT NULL COMMENT '角色标识',
  PRIMARY KEY (`id`),
  KEY `idx_adm_pr_assign_practitioner` (`practitioner_id`),
  KEY `idx_adm_pr_assign_org` (`org_id`),
  KEY `idx_adm_pr_assign_location` (`location_id`),
  KEY `idx_adm_pr_assign_role` (`role_id`),
  KEY `idx_adm_pr_assign_tenant` (`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='管理域 PR分配';

CREATE TABLE IF NOT EXISTS `adm_pr_assign_service` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT 0 COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  `version` int NOT NULL DEFAULT 0 COMMENT '乐观锁版本号',
  `assignment_id` varchar(64) NOT NULL COMMENT '岗位分配标识',
  `healthcare_service_id` varchar(64) NOT NULL COMMENT '医疗服务标识',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_adm_pr_assign_service` (`assignment_id`, `healthcare_service_id`),
  KEY `idx_adm_pr_assign_service_healthcare` (`healthcare_service_id`),
  KEY `idx_adm_pr_assign_service_tenant` (`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='管理域 PR分配服务';

-- 2.1 组织机构单元（whale_organization_units）
INSERT IGNORE INTO `whale_organization_units`
    (`id`, `code`, `display_name`, `parent_id`, `sort_order`, `is_enabled`, `remark`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('322663105157926912', 'OR0007',       '默认医院',   NULL, 0, 1, NULL, 1, '2026-06-09 09:07:59.06',  '2026-06-09 09:07:59.06',  1, 1, 0, NULL, NULL),
    ('322663218286694400', 'OR0007.OR0008', '门诊内科', '322663105157926912', 0, 1, NULL, 1, '2026-06-09 09:08:26.032', '2026-06-09 09:08:26.032', 1, 1, 0, NULL, NULL),
    ('322663308824940544', 'OR0007.OR0009', '住院内科', '322663105157926912', 0, 1, NULL, 1, '2026-06-09 09:08:47.618', '2026-06-09 09:08:47.618', 1, 1, 0, NULL, NULL),
    ('322663606398226432', 'OR0007.OR0010', '处置室',   '322663105157926912', 0, 1, NULL, 1, '2026-06-09 09:09:58.565', '2026-06-09 09:09:58.565', 1, 1, 0, NULL, NULL),
    ('322663652535570432', 'OR0007.OR0011', '护理站',   '322663105157926912', 0, 1, NULL, 1, '2026-06-09 09:10:09.565', '2026-06-09 09:10:09.565', 1, 1, 0, NULL, NULL),
    ('322663815438143488', 'OR0007.OR0012', '挂号收费处','322663105157926912', 0, 1, NULL, 1, '2026-06-09 09:10:48.405', '2026-06-09 09:10:48.405', 1, 1, 0, NULL, NULL);

-- 2.3 医院/科室（adm_organization）
INSERT IGNORE INTO `adm_organization`
    (`id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`,
     `deleted_at`, `deleted_by`, `version`, `name`, `bus_no`, `parent_id`, `type_enum`,
     `class_enum`, `status_enum`, `active_flag`, `display_order`)
VALUES
    ('322663105141149696', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '测试医院', 'OR0007', NULL, 'HOSPITAL', NULL, 'ACTIVE', 1, 1),
    ('322663218269917184', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '门诊内科', 'OR0007.OR0008', '322663105141149696', 'DEPARTMENT', 'CLINIC', 'ACTIVE', 1, 1),
    ('322663308812357632', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '住院内科', 'OR0007.OR0009', '322663105141149696', 'DEPARTMENT', 'INPATIENT', 'ACTIVE', 1, 2),
    ('322663606385643520', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '处置室',   'OR0007.OR0010', '322663105141149696', 'DEPARTMENT', 'NS', 'ACTIVE', 1, 3),
    ('322663652522987520', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '护理站',   'OR0007.OR0011', '322663105141149696', 'DEPARTMENT', 'NS', 'ACTIVE', 1, 4),
    ('322663815429754880', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '挂号收费处','OR0007.OR0012', '322663105141149696', 'DEPARTMENT', 'FIN', 'ACTIVE', 1, 5);

-- 2.4 系统用户（whale_users）
--    默认密码均为 123123（BCrypt），登录后可修改
INSERT IGNORE INTO `whale_users`
    (`id`, `user_name`, `email`, `phone_number`, `password_hash`, `name`, `surname`,
     `is_active`, `is_locked_out`, `lockout_end`, `access_failed_count`, `extra_properties`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`,
     `ukey_id`, `ukey_src_id`)
VALUES
    (1, 'admin', 'admin@example.com', NULL, '$2a$10$NFHHpR.QtQ/IW/S.2KOkAO/sJtY42FcrhA9nd9zjRLNUvxhmjliFi',
     'admin', NULL, 1, 0, NULL, 0, NULL, '2026-05-02 22:07:04.503', '2026-07-01 09:28:43.814731', NULL, NULL, 0, NULL, NULL, NULL, NULL),
    ('322932010666430464', 'yaofang', NULL, NULL, '$2a$10$fgewpcSdDLz1V8hz5mGJs.VI/AQSxgh15vCqQXv3uExk3MQQXeNq6',
     '药房', NULL, 1, 0, NULL, 0, NULL, '2026-06-10 02:56:31.129', '2026-06-10 02:56:31.129', 1, 1, 0, NULL, NULL, NULL, NULL),
    ('322932096091820032', 'kuguan', NULL, NULL, '$2a$10$xmtLIpu./wIOtQvtOCasN.CxUuSv0Z27KYkoK/KsmR0862gtZd2ne',
     '库管', NULL, 1, 0, NULL, 0, NULL, '2026-06-10 02:56:51.496', '2026-06-10 02:57:30.134', 1, 1, 0, NULL, NULL, NULL, NULL),
    ('322932211309350912', 'shoufei', NULL, NULL, '$2a$10$aUBhg.2LPcQaoaZJcqQSZusUsUxb52Sr9oPVhfE8.M6c40RPNs4di',
     '收费', NULL, 1, 0, NULL, 0, NULL, '2026-06-10 02:57:18.966', '2026-06-10 02:57:18.966', 1, 1, 0, NULL, NULL, NULL, NULL),
    ('322935698131259392', 'zyyisheng', NULL, NULL, '$2a$10$WLfRePjxQHwBcz5v/qpsPejS3I.bZU1PUKocrAL5rzn.fWGoDrvey',
     '住院医生', NULL, 1, 0, NULL, 0, NULL, '2026-06-10 03:11:10.289', '2026-06-10 03:11:10.289', 1, 1, 0, NULL, NULL, NULL, NULL),
    ('322963602154721280', 'yisheng', NULL, NULL, '$2a$10$40qkQYyXUxyemhox33jLMOaNht/hJnwdpA8ERsbdKN2lbXWUV0vBK',
     '门诊医生', NULL, 1, 0, NULL, 0, NULL, '2026-06-10 05:02:03.127', '2026-06-10 05:02:03.127', 1, 1, 0, NULL, NULL, NULL, NULL),
    ('322965022853566464', 'hushi', NULL, NULL, '$2a$10$L0qwEnhMmYg/UfpMnNGA1eaEy5NhM1LogeA8ehGQBSiO86ebOpsBm',
     '护士', NULL, 1, 0, NULL, 0, NULL, '2026-06-10 05:07:41.848', '2026-06-10 05:08:11.113', 1, 1, 0, NULL, NULL, NULL, NULL),
    ('323022664197214208', 'xinxike', NULL, NULL, '$2a$10$yUUyPkFp8YrLgFHB3ydovuoBwaqe6mF6dcs7PMpyImY4obRTFMSMS',
     '信息科', NULL, 1, 0, NULL, 0, NULL, '2026-06-10 08:56:44.615', '2026-06-10 08:56:44.615', 1, 1, 0, NULL, NULL, NULL, NULL);

-- 2.5 用户-角色关联（whale_user_roles）
INSERT IGNORE INTO `whale_user_roles`
    (`id`, `user_id`, `role_id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`,
     `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    (1, 1, 1, NULL, '2026-05-02 22:07:04.512', NULL, NULL, NULL, 0, NULL, NULL),
    ('322932010733539328', '322932010666430464', 12, 1, '2026-06-10 02:56:31.145', '2026-06-10 02:56:31.145', 1, 1, 0, NULL, NULL),
    ('322932096154734592', '322932096091820032', 13, 1, '2026-06-10 02:56:51.511', '2026-06-10 02:56:51.511', 1, 1, 0, NULL, NULL),
    ('322932211372265472', '322932211309350912', 14, 1, '2026-06-10 02:57:18.981', '2026-06-10 02:57:18.981', 1, 1, 0, NULL, NULL),
    ('322935698261282816', '322935698131259392', 10, 1, '2026-06-10 03:11:10.32', '2026-06-10 03:11:10.32', 1, 1, 0, NULL, NULL),
    ('322963602620289024', '322963602154721280', 10, 1, '2026-06-10 05:02:03.238', '2026-06-10 05:02:03.238', 1, 1, 0, NULL, NULL),
    ('322965022908092416', '322965022853566464', 11, 1, '2026-06-10 05:07:41.861', '2026-06-10 05:07:41.861', 1, 1, 0, NULL, NULL),
    ('322965878780989440', 1, 14, 1, '2026-06-10 05:11:05.917', '2026-06-10 05:11:05.917', 1, 1, 0, NULL, NULL),
    ('322965878881652736', 1, 10, 1, '2026-06-10 05:11:05.941', '2026-06-10 05:11:05.941', 1, 1, 0, NULL, NULL),
    ('322965878919401472', 1, 13, 1, '2026-06-10 05:11:05.95', '2026-06-10 05:11:05.95', 1, 1, 0, NULL, NULL),
    ('322965878990704640', 1, 12, 1, '2026-06-10 05:11:05.967', '2026-06-10 05:11:05.967', 1, 1, 0, NULL, NULL),
    ('322965879020064768', 1, 11, 1, '2026-06-10 05:11:05.974', '2026-06-10 05:11:05.974', 1, 1, 0, NULL, NULL),
    ('323021900313792512', '322965022853566464', 13, 1, '2026-06-10 08:53:42.491', '2026-06-10 08:53:42.491', 1, 1, 0, NULL, NULL),
    ('323022664272711680', '323022664197214208', '323022177255297024', 1, '2026-06-10 08:56:44.633', '2026-06-10 08:56:44.633', 1, 1, 0, NULL, NULL);

-- 2.6 用户-组织单元关联（whale_user_organization_units）
INSERT IGNORE INTO `whale_user_organization_units`
    (`id`, `user_id`, `organization_unit_id`, `tenant_id`, `created_at`, `updated_at`,
     `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('322961416313114624', '322935698131259392', '322663308824940544', 1, '2026-06-10 04:53:21.982', '2026-06-10 04:53:21.982', 1, 1, 0, NULL, NULL),
    ('322963602691592192', '322963602154721280', '322663218286694400', 1, '2026-06-10 05:02:03.255', '2026-06-10 05:02:03.255', 1, 1, 0, NULL, NULL),
    ('322965879032647680', 1, '322663218286694400', 1, '2026-06-10 05:11:05.977', '2026-06-10 05:11:05.977', 1, 1, 0, NULL, NULL),
    ('322965879032647681', 1, '322663308824940544', 1, '2026-06-10 05:11:05.977', '2026-06-10 05:11:05.977', 1, 1, 0, NULL, NULL),
    ('322972327045238784', '322965022853566464', '322663652535570432', 1, '2026-06-10 05:36:43.303', '2026-06-10 05:36:43.303', 1, 1, 0, NULL, NULL),
    ('322972706579943424', '322932211309350912', '322663815438143488', 1, '2026-06-10 05:38:13.791', '2026-06-10 05:38:13.791', 1, 1, 0, NULL, NULL),
    ('330237882234048512', '322932096091820032', '322663105157926912', 1, '2026-06-30 14:47:26.647648', '2026-06-30 14:47:26.647648', '322932096091820032', '322932096091820032', 0, NULL, NULL),
    ('330266733966266368', '322932010666430464', '322663105157926912', 1, '2026-06-30 16:42:05.436925', '2026-06-30 16:42:05.436925', '322932010666430464', '322932010666430464', 0, NULL, NULL);

-- 2.7 医务人员（adm_practitioner）
--    user_id 关联 whale_users.id，org_id 关联 adm_organization.id
INSERT IGNORE INTO `adm_practitioner`
    (`id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`,
     `deleted_at`, `deleted_by`, `version`, `name`, `gender_enum`, `status_enum`, `active_flag`,
     `user_id`, `org_id`)
VALUES
    ('308791351298756608', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     'admin',    'UNKNOWN', 'ACTIVE', 1, '1',                 '322663218269917184'),
    ('322932010683207680', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '药房',     'FEMALE',  'ACTIVE', 1, '322932010666430464', NULL),
    ('322932096108597248', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '库管',     'FEMALE',  'ACTIVE', 1, '322932096091820032', NULL),
    ('322932211326128128', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '收费',     'FEMALE',  'ACTIVE', 1, '322932211309350912', '322663815429754880'),
    ('322935698173202432', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '住院医生', 'MALE',    'ACTIVE', 1, '322935698131259392', '322663308812357632'),
    ('322963602326687744', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '门诊医生', 'MALE',    'ACTIVE', 1, '322963602154721280', '322663218269917184'),
    ('322965022887120896', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '护士',     'FEMALE',  'ACTIVE', 1, '322965022853566464', '322663652522987520'),
    ('323022664260128768', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '信息科',   'MALE',    'ACTIVE', 1, '323022664197214208', NULL);

-- 2.8 号源服务（adm_healthcare_service）
INSERT IGNORE INTO `adm_healthcare_service`
    (`id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`,
     `deleted_at`, `deleted_by`, `version`, `name`, `type_code`, `category_code`,
     `offered_org_id`, `specialty_code`, `location_id`, `status_enum`, `active_flag`,
     `appointment_required_flag`, `yb_no`, `bus_no`, `comment`, `extra_details`, `contact`)
VALUES
    ('322957224354254848', 1, '2026-06-10 12:36:42.541', '2026-06-10 14:57:21.25', 1, 1, 0, NULL, NULL, 0,
     '便民号', '1', NULL, NULL, NULL, NULL, 'ACTIVE', 1, NULL, '001', 'HS0001', NULL, NULL, NULL),
    ('322957264586018816', 1, '2026-06-10 12:36:52.133', '2026-06-10 14:57:31.63', 1, 1, 0, NULL, NULL, 0,
     '普通号', '2', NULL, NULL, NULL, NULL, 'ACTIVE', 1, NULL, '002', 'HS0002', NULL, NULL, NULL),
    ('322957296852799488', 1, '2026-06-10 12:36:59.826', '2026-06-10 14:57:36.527', 1, 1, 0, NULL, NULL, 0,
     '专家号', '3', NULL, NULL, NULL, NULL, 'ACTIVE', 1, NULL, '003', 'HS0003', NULL, NULL, NULL);

-- 2.9 地点（adm_location）
--    form_enum: WARD=病区, PHARMACY=药房, WAREHOUSE=库房
INSERT IGNORE INTO `adm_location`
    (`id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`,
     `deleted_at`, `deleted_by`, `version`, `name`, `bus_no`, `status_enum`, `operational_enum`,
     `form_enum`, `mode_enum`, `type_json`, `organization_id`, `display_order`)
VALUES
    ('322954857622802432', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '耗材库', 'LO0019', 'ACTIVE', 'NOT_IN_USE', 'WAREHOUSE', 'INSTANCE', '{}', '322663105141149696', 0),
    ('322954885380706304', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '西药库', 'LO0020', 'ACTIVE', 'NOT_IN_USE', 'WAREHOUSE', 'INSTANCE', '{}', '322663105141149696', 0),
    ('322954919841107968', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '中药库', 'LO0021', 'ACTIVE', 'NOT_IN_USE', 'WAREHOUSE', 'INSTANCE', '{}', '322663105141149696', 0),
    ('322954949549363200', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '门诊西药房', 'LO0022', 'ACTIVE', 'NOT_IN_USE', 'PHARMACY', 'INSTANCE', '{}', '322663105141149696', 0),
    ('322954984441778176', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '门诊中药房', 'LO0023', 'ACTIVE', 'NOT_IN_USE', 'PHARMACY', 'INSTANCE', '{}', '322663105141149696', 0),
    ('322955031114878976', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '门诊内科病区', 'LO0024', 'ACTIVE', 'NOT_IN_USE', 'WARD', 'INSTANCE', '{}', '322663105141149696', 0),
    ('322955071420633088', 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL, 0,
     '住院内科病区', 'LO0025', 'ACTIVE', 'NOT_IN_USE', 'WARD', 'INSTANCE', '{}', '322663105141149696', 0);

-- 用户租户关系。角色和组织关系不能代替租户关系，登录和用户查询会独立读取此表。
INSERT INTO `whale_user_tenants`
    (`id`, `user_id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`,
     `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    ('5', '1', '1', '2026-05-02 22:07:04.564', '2026-05-02 22:07:04.564', '1', NULL, 0, NULL, NULL),
    ('322934748440170496', '322932211309350912', '1', '2026-06-10 03:07:23.865', '2026-06-10 03:07:23.865', '1', '1', 0, NULL, NULL),
    ('322934748461142016', '322932096091820032', '1', '2026-06-10 03:07:23.870', '2026-06-10 03:07:23.870', '1', '1', 0, NULL, NULL),
    ('322934748473724928', '322932010666430464', '1', '2026-06-10 03:07:23.873', '2026-06-10 03:07:23.873', '1', '1', 0, NULL, NULL),
    ('322963602188275712', '322963602154721280', '1', '2026-06-10 05:02:03.135', '2026-06-10 05:02:03.135', '1', '1', 0, NULL, NULL),
    ('322965022861955072', '322965022853566464', '1', '2026-06-10 05:07:41.850', '2026-06-10 05:07:41.850', '1', '1', 0, NULL, NULL),
    ('323022664205602816', '323022664197214208', '1', '2026-06-10 08:56:44.617', '2026-06-10 08:56:44.617', '1', '1', 0, NULL, NULL),
    ('330552204578525184', '322935698131259392', '1', '2026-07-01 11:36:26.934173', '2026-07-01 11:36:26.934173', '1', '1', 0, NULL, NULL)
ON DUPLICATE KEY UPDATE `id` = `whale_user_tenants`.`id`;

-- 仅初始化引用已在本脚本中存在的机构、地点、角色和医务人员，避免部署后产生孤立关系。
INSERT INTO `adm_pr_assign`
    (`id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`,
     `deleted_at`, `deleted_by`, `version`, `name`, `active_flag`, `practitioner_id`, `org_id`,
     `location_id`, `specialty_code`, `type_code`, `role_id`)
VALUES
    ('322965878747435008', '1', '2026-06-10 13:11:05.909', '2026-06-10 13:11:05.909', '1', '1', 0, NULL, NULL, 0,
     'admin', 1, '308791351298756608', '322663218269917184', NULL, NULL, NULL, '14'),
    ('322965878747435009', '1', '2026-06-10 13:11:05.909', '2026-06-10 13:11:05.909', '1', '1', 0, NULL, NULL, 0,
     'admin', 1, '308791351298756608', '322663308812357632', NULL, NULL, NULL, '14'),
    ('322972360645808128', '1', '2026-06-10 13:36:51.314', '2026-06-10 13:36:51.314', '1', '1', 0, NULL, NULL, 0,
     '门诊医生', 1, '322963602326687744', '322663218269917184', NULL, NULL, NULL, '14'),
    ('322972360708722688', '1', '2026-06-10 13:36:51.329', '2026-06-10 13:36:51.329', '1', '1', 0, NULL, NULL, 0,
     '门诊医生', 1, '322963602326687744', '322663218269917184', NULL, NULL, NULL, '10'),
    ('322972376974233600', '1', '2026-06-10 13:36:55.207', '2026-06-10 13:36:55.207', '1', '1', 0, NULL, NULL, 0,
     '住院医生', 1, '322935698173202432', '322663308812357632', NULL, NULL, NULL, '14'),
    ('322977240106668032', '1', '2026-06-10 13:56:14.668', '2026-06-10 13:56:14.668', '1', '1', 0, NULL, NULL, 0,
     '药房', 1, '322932010683207680', NULL, '322954949549363200', NULL, NULL, '12')
ON DUPLICATE KEY UPDATE `id` = `adm_pr_assign`.`id`;

INSERT INTO `adm_pr_assign_service`
    (`id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`,
     `deleted_at`, `deleted_by`, `version`, `assignment_id`, `healthcare_service_id`)
VALUES
    ('322972360708722688', '1', '2026-06-10 13:36:51.329', '2026-06-10 13:36:51.329', '1', '1', 0, NULL, NULL, 0,
     '322972360708722688', '322957224354254848'),
    ('322972360708722689', '1', '2026-06-10 13:36:51.329', '2026-06-10 13:36:51.329', '1', '1', 0, NULL, NULL, 0,
     '322972360708722688', '322957264586018816'),
    ('322972360708722690', '1', '2026-06-10 13:36:51.329', '2026-06-10 13:36:51.329', '1', '1', 0, NULL, NULL, 0,
     '322972360708722688', '322957296852799488')
ON DUPLICATE KEY UPDATE `id` = `adm_pr_assign_service`.`id`;

-- =====================================================================
-- 六、部署后核对（只读）
-- =====================================================================

SELECT 'whale_menus' AS `table_name`, COUNT(*) AS `row_count` FROM `whale_menus`
UNION ALL SELECT 'whale_roles', COUNT(*) FROM `whale_roles`
UNION ALL SELECT 'whale_role_claims', COUNT(*) FROM `whale_role_claims`
UNION ALL SELECT 'whale_permissions', COUNT(*) FROM `whale_permissions`
UNION ALL SELECT 'whale_permission_grants', COUNT(*) FROM `whale_permission_grants`
UNION ALL SELECT 'whale_organization_units', COUNT(*) FROM `whale_organization_units`
UNION ALL SELECT 'adm_organization', COUNT(*) FROM `adm_organization`
UNION ALL SELECT 'whale_users', COUNT(*) FROM `whale_users`
UNION ALL SELECT 'whale_user_roles', COUNT(*) FROM `whale_user_roles`
UNION ALL SELECT 'whale_user_tenants', COUNT(*) FROM `whale_user_tenants`
UNION ALL SELECT 'whale_user_organization_units', COUNT(*) FROM `whale_user_organization_units`
UNION ALL SELECT 'adm_practitioner', COUNT(*) FROM `adm_practitioner`
UNION ALL SELECT 'adm_healthcare_service', COUNT(*) FROM `adm_healthcare_service`
UNION ALL SELECT 'adm_location', COUNT(*) FROM `adm_location`
UNION ALL SELECT 'adm_pr_assign', COUNT(*) FROM `adm_pr_assign`
UNION ALL SELECT 'adm_pr_assign_service', COUNT(*) FROM `adm_pr_assign_service`;

SELECT `id`, `name`, `display_name`, `code`, `is_default`, `is_static`, `tenant_id`, `is_deleted`
FROM `whale_roles`
WHERE `name` = 'admin';

SELECT COUNT(*) AS `orphan_role_claim_count`
FROM `whale_role_claims` rc
LEFT JOIN `whale_roles` r ON r.`id` = rc.`role_id`
WHERE r.`id` IS NULL;

SELECT COUNT(*) AS `orphan_permission_grant_count`
FROM `whale_permission_grants` pg
LEFT JOIN `whale_permissions` p ON p.`name` = pg.`permission_name`
LEFT JOIN `whale_roles` r ON pg.`provider_name` = 'R' AND r.`id` = pg.`provider_key`
WHERE p.`id` IS NULL OR (pg.`provider_name` = 'R' AND r.`id` IS NULL);

SELECT COUNT(*) AS `orphan_user_role_count`
FROM `whale_user_roles` ur
LEFT JOIN `whale_users` u ON u.`id` = ur.`user_id`
LEFT JOIN `whale_roles` r ON r.`id` = ur.`role_id`
WHERE u.`id` IS NULL OR r.`id` IS NULL;

SELECT COUNT(*) AS `orphan_user_tenant_count`
FROM `whale_user_tenants` ut
LEFT JOIN `whale_users` u ON u.`id` = ut.`user_id`
WHERE u.`id` IS NULL;

SELECT COUNT(*) AS `orphan_user_organization_count`
FROM `whale_user_organization_units` uou
LEFT JOIN `whale_users` u ON u.`id` = uou.`user_id`
LEFT JOIN `whale_organization_units` ou ON ou.`id` = uou.`organization_unit_id`
WHERE u.`id` IS NULL OR ou.`id` IS NULL;

SELECT COUNT(*) AS `orphan_practitioner_count`
FROM `adm_practitioner` p
LEFT JOIN `whale_users` u ON u.`id` = p.`user_id`
WHERE u.`id` IS NULL;

SELECT COUNT(*) AS `orphan_pr_assign_count`
FROM `adm_pr_assign` pa
LEFT JOIN `adm_practitioner` p ON p.`id` = pa.`practitioner_id`
LEFT JOIN `whale_roles` r ON r.`id` = pa.`role_id`
LEFT JOIN `adm_organization` o ON o.`id` = pa.`org_id`
LEFT JOIN `adm_location` l ON l.`id` = pa.`location_id`
WHERE p.`id` IS NULL OR r.`id` IS NULL
   OR (pa.`org_id` IS NOT NULL AND o.`id` IS NULL)
   OR (pa.`location_id` IS NOT NULL AND l.`id` IS NULL);

SELECT COUNT(*) AS `orphan_pr_assign_service_count`
FROM `adm_pr_assign_service` pas
LEFT JOIN `adm_pr_assign` pa ON pa.`id` = pas.`assignment_id`
LEFT JOIN `adm_healthcare_service` hs ON hs.`id` = pas.`healthcare_service_id`
WHERE pa.`id` IS NULL OR hs.`id` IS NULL;

SET FOREIGN_KEY_CHECKS = @OLD_FOREIGN_KEY_CHECKS;
