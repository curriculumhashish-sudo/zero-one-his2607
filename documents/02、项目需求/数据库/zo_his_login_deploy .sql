-- =====================================================================
-- 文件名 : zo_his_login_deploy.sql
-- 用途   : his-j1-login 登录模块 · 阿里云服务器部署初始化脚本
--
-- 内容   :
--   1) 建库 zo_his（不存在时创建）
--   2) 登录模块依赖的 11 张表 DDL（与云效标准库 zo_his.sql 结构一致）
--   3) 索引与外键约束（与 zo_his_idx.sql 一致）
--   4) 初始化数据：
--      - 用户 / 用户角色 / 用户租户 / 用户科室关联（源自 zo_his_init_user.sql）
--      - 系统角色定义 whale_roles（源自云效标准注释，登录角色链路必需）
--      - 租户 / 组织单元（源自 zo_his_init_sys.sql）
--      - 医院 adm_organization / 医务人员 adm_practitioner（登录后医院选择与用户信息必需）
--
-- 特性   : 幂等安全（CREATE TABLE IF NOT EXISTS / INSERT IGNORE），可重复执行
--
-- 注意   :
--   本文件仅覆盖 his-j1-login 登录链路相关表；完整业务库表请另行执行 zo_his.sql。
--   执行前建议：mysql -uroot -p < zo_his_login_deploy.sql
-- =====================================================================

-- 建库（不存在时创建）
CREATE DATABASE IF NOT EXISTS `zo_his`
    DEFAULT CHARACTER SET utf8mb4
    DEFAULT COLLATE utf8mb4_general_ci;

USE `zo_his`;
SET NAMES utf8mb4;
SET @OLD_FOREIGN_KEY_CHECKS = @@FOREIGN_KEY_CHECKS;
SET FOREIGN_KEY_CHECKS = 0;

-- =====================================================================
-- 一、表结构（DDL）
-- =====================================================================

-- 1.1 系统用户表（登录账号）
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

-- 1.2 系统角色表（登录角色加载）
CREATE TABLE IF NOT EXISTS `whale_roles` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `name` varchar(256) NOT NULL COMMENT '角色名称（唯一）',
  `display_name` varchar(256) DEFAULT NULL COMMENT '显示名称',
  `code` varchar(256) DEFAULT NULL COMMENT '角色编码',
  `description` text COMMENT '描述',
  `is_default` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否默认角色',
  `is_static` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否静态角色（不可删）',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
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

-- 1.3 租户表
CREATE TABLE IF NOT EXISTS `whale_tenants` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `name` varchar(128) NOT NULL COMMENT '租户名称',
  `code` varchar(64) NOT NULL COMMENT '租户代码',
  `description` varchar(512) DEFAULT NULL COMMENT '租户描述',
  `is_active` tinyint(1) NOT NULL DEFAULT '1' COMMENT '是否启用',
  `extra_properties` text COMMENT '扩展属性',
  `created_at` datetime(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  PRIMARY KEY (`id`),
  UNIQUE KEY `whale_tenants_code_key` (`code`),
  KEY `idx_whale_tenants_code` (`code`),
  KEY `idx_whale_tenants_created_at` (`created_at`),
  KEY `idx_whale_tenants_is_active` (`is_active`),
  KEY `idx_whale_tenants_is_deleted` (`is_deleted`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='租户';

-- 1.4 用户-角色关联表
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

-- 1.5 用户-租户关联表
CREATE TABLE IF NOT EXISTS `whale_user_tenants` (
  `id` varchar(64) NOT NULL COMMENT '主键',
  `user_id` varchar(64) NOT NULL COMMENT '用户标识',
  `tenant_id` varchar(64) NOT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_user_tenant` (`user_id`,`tenant_id`),
  KEY `idx_whale_user_tenants_is_deleted` (`is_deleted`),
  KEY `idx_whale_user_tenants_tenant_id` (`tenant_id`),
  KEY `idx_whale_user_tenants_user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='用户租户关联';

-- 1.6 组织机构单元表（科室）
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

-- 1.7 用户-组织单元关联表（用户可选科室）
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

-- 1.8 用户登录会话表（租户/医院/科室选择持久化）
CREATE TABLE IF NOT EXISTS `whale_user_login_session` (
  `user_id` varchar(64) NOT NULL COMMENT '用户标识',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `hospital_organization_id` varchar(64) DEFAULT NULL COMMENT 'hospital机构标识',
  `organization_unit_id` varchar(64) DEFAULT NULL COMMENT '机构单元标识',
  `updated_at` datetime(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) COMMENT '最后更新时间',
  PRIMARY KEY (`user_id`),
  KEY `idx_whale_user_login_session_tenant` (`tenant_id`),
  CONSTRAINT `fk_whale_user_login_session_user` FOREIGN KEY (`user_id`) REFERENCES `whale_users` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='用户登录后的服务端会话：租户 / 医院 / 科室选择（跨重启持久化）';

-- 1.9 管理域机构表（医院/科室，登录后医院选择）
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

-- 1.10 医务人员表（登录后用户信息 practitioner 字段）
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
  `name_json` varchar(2000) DEFAULT NULL COMMENT '其他名称 JSON',
  `py_str` varchar(255) DEFAULT NULL COMMENT '姓名拼音检索码',
  `wb_str` varchar(255) DEFAULT NULL COMMENT '姓名五笔检索码',
  `gender_enum` varchar(255) NOT NULL COMMENT '行政性别编码',
  `birth_date` date DEFAULT NULL COMMENT '出生日期',
  `deceased_date` datetime(6) DEFAULT NULL COMMENT '死亡时间',
  `phone` varchar(50) DEFAULT NULL COMMENT '联系电话',
  `address_province` varchar(64) DEFAULT NULL COMMENT '地址-省',
  `address_city` varchar(64) DEFAULT NULL COMMENT '地址-市',
  `address_district` varchar(64) DEFAULT NULL COMMENT '地址-区/县',
  `address_street` varchar(255) DEFAULT NULL COMMENT '地址-街道/详细地址',
  `address_json` varchar(2000) DEFAULT NULL COMMENT '地址扩展 JSON',
  `status_enum` varchar(255) NOT NULL COMMENT '人员状态（ACTIVE / INACTIVE / DISCARDED）',
  `active_flag` tinyint(1) NOT NULL DEFAULT '1' COMMENT '是否启用',
  `bus_no` varchar(64) DEFAULT NULL COMMENT '院内工号/业务编号',
  `yb_no` varchar(64) DEFAULT NULL COMMENT '医保人员编码',
  `user_id` varchar(64) NOT NULL COMMENT '关联平台用户主键',
  `org_id` varchar(64) DEFAULT NULL COMMENT '默认登录科室',
  `dr_profttl_code` int DEFAULT NULL COMMENT '职称编码',
  `phar_prac_cert_no` varchar(64) DEFAULT NULL COMMENT '医师/药师执业资格证编号',
  `prsc_dr_cert_code` varchar(64) DEFAULT NULL COMMENT '执业证件类型编码',
  `signature` text COMMENT '医生签名图片 Base64',
  `kpd_code` varchar(64) DEFAULT NULL COMMENT '开票点编码',
  `pos_no` varchar(64) DEFAULT NULL COMMENT 'POS 机终端编号',
  PRIMARY KEY (`id`),
  KEY `idx_adm_practitioner_user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='医务人员主数据';

-- 1.11 菜单表（登录后菜单权限）
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
  `sort_order` int DEFAULT '0' COMMENT '排序号',
  `is_enabled` tinyint(1) NOT NULL DEFAULT '1' COMMENT '是否启用',
  `is_hidden` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否隐藏',
  `is_external` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否外链',
  `external_url` varchar(512) DEFAULT NULL COMMENT '外链 URL',
  `menu_type` varchar(32) NOT NULL DEFAULT 'menu' COMMENT '菜单类型（menu/directory）',
  `remark` varchar(512) DEFAULT NULL COMMENT '备注',
  `tenant_id` varchar(64) DEFAULT NULL COMMENT '租户标识',
  `created_at` datetime(6) NOT NULL COMMENT '创建时间',
  `updated_at` datetime(6) DEFAULT NULL COMMENT '最后更新时间',
  `created_by` varchar(64) DEFAULT NULL COMMENT '创建人',
  `updated_by` varchar(64) DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT '0' COMMENT '软删标记',
  `deleted_at` datetime(6) DEFAULT NULL COMMENT '删除时间',
  `deleted_by` varchar(64) DEFAULT NULL COMMENT '删除人',
  `keep_alive` tinyint(1) DEFAULT NULL COMMENT '是否缓存页面',
  PRIMARY KEY (`id`),
  UNIQUE KEY `whale_menus_code_key` (`code`(64)),
  KEY `idx_whale_menus_is_enabled` (`is_enabled`),
  KEY `idx_whale_menus_menu_type` (`menu_type`),
  KEY `idx_whale_menus_parent_id` (`parent_id`),
  KEY `idx_whale_menus_tenant_id` (`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='菜单';

-- =====================================================================
-- 二、初始化数据（DML）
-- =====================================================================

-- 2.1 租户（whale_tenants）
INSERT IGNORE INTO `whale_tenants`
    (`id`, `name`, `code`, `description`, `is_active`, `extra_properties`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    (1, '默认租户', 'DEFAULT', '系统默认租户，用于单租户或主租户场景', 1, NULL,
     '2026-05-02 22:07:04.556', '2026-05-02 22:07:04.556', 1, 1, 0, NULL, NULL);

-- 2.2 系统角色（whale_roles）
--    注意：whale_user_roles 引用的角色 ID 必须在此定义，否则角色加载为空
INSERT IGNORE INTO `whale_roles`
    (`id`, `name`, `display_name`, `code`, `description`, `is_default`, `is_static`, `tenant_id`,
     `created_at`, `updated_at`, `created_by`, `updated_by`, `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    (1,   'admin',            '系统管理员', 'ROLE_ADMIN',          '系统管理员，拥有所有权限', 0, 1, 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    (10,  'doctor',           '医生',       'ROLE_DOCTOR',         '医生角色，负责诊疗相关工作', 1, 1, 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    (11,  'nurse',            '护士',       'ROLE_NURSE',          '护士角色，负责护理相关工作', 1, 1, 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    (12,  'pharmacist',       '药师',       'ROLE_PHARMACIST',     '药师角色，负责药房药品管理', 1, 1, 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    (13,  'warehouse_keeper', '库管',       'ROLE_WAREHOUSE',      '库管角色，负责库存物资管理', 1, 1, 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    (14,  'accountant',       '财务',       'ROLE_ACCOUNTANT',     '财务角色，负责财务收费管理', 1, 1, 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    -- 业务自定义角色（whale_user_roles / user_role 已引用；名称/编码请按业务校准）
    ('323022177255297024', '信息科管理员', '信息科', 'ROLE_IT_ADMIN',
     '信息科管理角色（权限授权数据已存在于 whale_permission_grants）', 0, 0, 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL),
    ('329631932614709248', '库存管理员',   '库管扩展', 'ROLE_STOCK_ADMIN',
     '库存管理扩展角色', 0, 0, 1, NOW(6), NOW(6), '1', '1', 0, NULL, NULL);

-- 2.3 系统用户（whale_users，源自 zo_his_init_user.sql）
--    默认密码均可在登录后通过 /user/changePassword 修改
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

-- 2.4 用户-角色关联（whale_user_roles，源自 zo_his_init_user.sql，含历史软删记录）
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
    ('323022664272711680', '323022664197214208', '323022177255297024', 1, '2026-06-10 08:56:44.633', '2026-06-10 08:56:44.633', 1, 1, 0, NULL, NULL),
    ('329632199905120256', 1, '329631932614709248', 1, '2026-06-28 22:40:40.727796', '2026-06-28 22:59:22.074157', 1, 1, 1, '2026-06-28 22:59:22.059617', NULL),
    ('329632661576355840', '323022664197214208', '329631932614709248', 1, '2026-06-28 22:42:30.798728', '2026-06-28 22:59:33.267434', 1, 1, 1, '2026-06-28 22:59:33.263977', NULL),
    ('330237846779596800', '322932096091820032', 12, 1, '2026-06-30 14:47:18.194657', '2026-06-30 14:47:18.231661', 1, 1, 1, '2026-06-30 14:47:18.225658', NULL);

-- 2.5 用户-租户关联（whale_user_tenants，源自 zo_his_init_user.sql）
INSERT IGNORE INTO `whale_user_tenants`
    (`id`, `user_id`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`,
     `is_deleted`, `deleted_at`, `deleted_by`)
VALUES
    (5, 1, 1, '2026-05-02 22:07:04.564', '2026-05-02 22:07:04.564', 1, NULL, 0, NULL, NULL),
    ('322934748440170496', '322932211309350912', 1, '2026-06-10 03:07:23.865', '2026-06-10 03:07:23.865', 1, 1, 0, NULL, NULL),
    ('322934748461142016', '322932096091820032', 1, '2026-06-10 03:07:23.87', '2026-06-10 03:07:23.87', 1, 1, 0, NULL, NULL),
    ('322934748473724928', '322932010666430464', 1, '2026-06-10 03:07:23.873', '2026-06-10 03:07:23.873', 1, 1, 0, NULL, NULL),
    ('322963602188275712', '322963602154721280', 1, '2026-06-10 05:02:03.135', '2026-06-10 05:02:03.135', 1, 1, 0, NULL, NULL),
    ('322965022861955072', '322965022853566464', 1, '2026-06-10 05:07:41.85', '2026-06-10 05:07:41.85', 1, 1, 0, NULL, NULL),
    ('323022664205602816', '323022664197214208', 1, '2026-06-10 08:56:44.617', '2026-06-10 08:56:44.617', 1, 1, 0, NULL, NULL),
    ('330552204578525184', '322935698131259392', 1, '2026-07-01 11:36:26.934173', '2026-07-01 11:36:26.934173', 1, 1, 0, NULL, NULL);

-- 2.6 组织机构单元（whale_organization_units，源自 zo_his_init_sys.sql）
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

-- 2.7 用户-组织单元关联（whale_user_organization_units，源自 zo_his_init_user.sql）
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

-- 2.8 医院/科室（adm_organization，登录后医院选择）
--    注意：type_enum/status_enum 使用标准枚举编码 HOSPITAL/ACTIVE，
--    代码侧 TenantServiceImpl 需使用 'HOSPITAL'/'ACTIVE'（而非 '医院'/'生效'）
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

-- 2.9 医务人员（adm_practitioner，登录后 /user/info practitioner 字段）
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

-- 2.10 菜单（whale_menus 基础项，源自 zo_his_init_sys.sql，可按需增补）
INSERT IGNORE INTO `whale_menus`
    (`id`, `code`, `name`, `display_name`, `icon`, `path`, `component`, `permission_name`,
     `parent_id`, `sort_order`, `is_enabled`, `is_hidden`, `is_external`, `external_url`,
     `menu_type`, `remark`, `tenant_id`, `created_at`, `updated_at`, `created_by`, `updated_by`,
     `is_deleted`, `deleted_at`, `deleted_by`, `keep_alive`)
VALUES
    (1000, 'system', 'System', '系统管理', 'lucide:settings', '/system', '/system/index', 'Navigation.System',
     NULL, 0, 1, 0, 0, '', 'directory', '', 1, '2026-05-26 20:00:00', '2026-05-26 20:00:00', 1, 1, 0, NULL, NULL, 1),
    (1001, 'user_management', 'UserManagement', '用户管理', 'lucide:user', '/system/user', '/system/user/index', 'Navigation.UserManagement',
     1000, 1, 1, 0, 0, '', 'menu', '', 1, '2026-05-26 20:00:00', '2026-05-26 20:00:00', 1, 1, 0, NULL, NULL, 1),
    (1002, 'role_management', 'RoleManagement', '角色管理', 'lucide:shield-check', '/system/user/role', '/system/user/role/index', 'Navigation.RoleManagement',
     1000, 2, 1, 0, 0, '', 'menu', '', 1, '2026-05-26 20:00:00', '2026-05-26 20:00:00', 1, 1, 0, NULL, NULL, 1),
    (1004, 'permission_management', 'PermissionManagement', '权限管理', 'lucide:fingerprint-pattern', '/system/user/permission', '/system/user/permission/index', 'Navigation.PermissionManagement',
     1000, 3, 1, 0, 0, '', 'menu', '', 1, '2026-05-26 20:00:00', '2026-05-26 20:00:00', 1, 1, 0, NULL, NULL, 1);

-- =====================================================================
-- 三、部署后核对（只读）
-- =====================================================================
SELECT 'whale_users' AS tbl, COUNT(*) AS rows_ FROM whale_users
UNION ALL SELECT 'whale_roles', COUNT(*) FROM whale_roles
UNION ALL SELECT 'whale_user_roles', COUNT(*) FROM whale_user_roles
UNION ALL SELECT 'whale_user_tenants', COUNT(*) FROM whale_user_tenants
UNION ALL SELECT 'whale_organization_units', COUNT(*) FROM whale_organization_units
UNION ALL SELECT 'whale_user_organization_units', COUNT(*) FROM whale_user_organization_units
UNION ALL SELECT 'whale_tenants', COUNT(*) FROM whale_tenants
UNION ALL SELECT 'adm_organization', COUNT(*) FROM adm_organization
UNION ALL SELECT 'adm_practitioner', COUNT(*) FROM adm_practitioner
UNION ALL SELECT 'whale_menus', COUNT(*) FROM whale_menus;

SET FOREIGN_KEY_CHECKS = @OLD_FOREIGN_KEY_CHECKS;
--解释：
-- =====================================================================
-- 部署后代码适配清单（本脚本不涉及，必须同步完成）
--  1) his-oauth2/RoleMapper.xml + entity/Role.java → 查询 whale_roles / whale_user_roles（加 @TableName("whale_roles")）
--  2) his-j1-login/TenantServiceImpl.getHospitalList() → type_enum='HOSPITAL'、status_enum='ACTIVE'（当前代码为 '医院'/'生效'）
--  3) his-j1-login/mapper/dept/DeptMapper.xml → namespace=com.zeroone.star.sysmgr.mapper.dept.DeptMapper
--     → resultType=com.zeroone.star.sysmgr.entity.dept.WhaleOrganizationUnitDO
--  4) his-j1-login/config/MyBatisInit.java → 自动填充 created_at/updated_at
--  5) TenantDO/UserTenantDO/TenantVO/HospitalVO 等 ID 类型 Long → String
--  6) cacheSelectedTenant/cacheSelectedHospital 同步持久化 whale_user_login_session
--  7) LoginUserMapper.selectProfileById 增加 p.status_enum='ACTIVE' 且 LIMIT 1
-- =====================================================================
