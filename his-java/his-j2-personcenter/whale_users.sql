/*
 Navicat Premium Data Transfer

 Source Server         : 192.168.23.136
 Source Server Type    : MySQL
 Source Server Version : 80409 (8.4.9)
 Source Host           : 192.168.23.136:3306
 Source Schema         : test

 Target Server Type    : MySQL
 Target Server Version : 80409 (8.4.9)
 File Encoding         : 65001

 Date: 25/07/2026 13:12:02
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for whale_users
-- ----------------------------
DROP TABLE IF EXISTS `whale_users`;
CREATE TABLE `whale_users`  (
  `user_id` bigint NOT NULL COMMENT '主键',
  `tenant_id` bigint NULL DEFAULT NULL COMMENT '租户ID',
  `user_name` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '登录用户名',
  `real_name` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '真实姓名',
  `avatar` varchar(512) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '头像URL',
  `email` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '邮箱',
  `phone_number` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '手机号',
  `introduction` varchar(512) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '个人简介',
  `password_hash` varchar(256) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL COMMENT '密码哈希',
  `is_active` tinyint(1) NOT NULL DEFAULT 1 COMMENT '是否启用',
  `is_locked_out` tinyint(1) NOT NULL DEFAULT 0 COMMENT '是否锁定',
  `lockout_end` datetime NULL DEFAULT NULL COMMENT '锁定结束时间',
  `access_failed_count` int NOT NULL DEFAULT 0 COMMENT '登录失败次数',
  `organization_id` bigint NULL DEFAULT NULL COMMENT '所属机构ID',
  `practitioner_id` bigint NULL DEFAULT NULL COMMENT '所属执业人员ID',
  `created_at` datetime NOT NULL COMMENT '创建时间',
  `updated_at` datetime NULL DEFAULT NULL COMMENT '最后更新时间',
  `created_by` bigint NULL DEFAULT NULL COMMENT '创建人',
  `updated_by` bigint NULL DEFAULT NULL COMMENT '最后更新人',
  `is_deleted` tinyint(1) NOT NULL DEFAULT 0 COMMENT '软删标记',
  `deleted_at` datetime NULL DEFAULT NULL COMMENT '删除时间',
  `deleted_by` bigint NULL DEFAULT NULL COMMENT '删除人',
  `version` int NOT NULL DEFAULT 0 COMMENT '乐观锁版本号',
  PRIMARY KEY (`user_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci COMMENT = '系统用户' ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
