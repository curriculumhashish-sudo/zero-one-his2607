-- =====================================================================
-- 文件名 : zo_sysmgr_init_sys.sql
-- 用途   : his-j1-sysmgr 系统管理微服务部署初始化脚本（阶段稿）
-- 编码   : UTF-8
--
-- 当前覆盖：
--   1) 菜单管理：whale_menus 表结构与系统管理基础菜单
--   2) 角色管理：whale_roles、whale_role_claims 表结构与基础角色数据
--
-- 待其他阶段补充：
--   1) 权限管理：whale_permissions、whale_permission_grants
--   2) 用户管理：whale_users、whale_user_roles、用户租户及组织关系等
--
-- 安全约束：
--   1) 不使用 DROP TABLE/TRUNCATE，避免破坏共享环境已有数据
--   2) DDL 使用 IF NOT EXISTS，初始化数据可重复执行
--   3) 表结构以 documents/02、项目需求/数据库/zo_his.sql 为准
--
-- 执行前必须确认 his-test 的 JDBC URL 指向 zo_his；若实际库名不同，
-- 请由数据库负责人审核并修改下面的 USE 语句后再执行。
-- =====================================================================

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
    ('17', 'finance_approver',   '财务审批',   'ROLE_FINANCE_APPROVER', '财务审批岗位', 1, 1, '1', NOW(6), NOW(6), '1', '1', 0, NULL, NULL)
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
-- 三、尚未整理的系统管理模块
-- =====================================================================

-- TODO 权限管理：由后续任务补充 whale_permissions、whale_permission_grants。
-- TODO 用户管理：由后续任务补充用户、用户角色、用户租户及用户组织关系等表和初始数据。

-- =====================================================================
-- 四、部署后核对（只读）
-- =====================================================================

SELECT 'whale_menus' AS `table_name`, COUNT(*) AS `row_count` FROM `whale_menus`
UNION ALL
SELECT 'whale_roles', COUNT(*) FROM `whale_roles`
UNION ALL
SELECT 'whale_role_claims', COUNT(*) FROM `whale_role_claims`;

SELECT `id`, `name`, `display_name`, `code`, `is_default`, `is_static`, `tenant_id`, `is_deleted`
FROM `whale_roles`
WHERE `name` = 'admin';

SELECT COUNT(*) AS `orphan_role_claim_count`
FROM `whale_role_claims` rc
LEFT JOIN `whale_roles` r ON r.`id` = rc.`role_id`
WHERE r.`id` IS NULL;

SET FOREIGN_KEY_CHECKS = @OLD_FOREIGN_KEY_CHECKS;
