package com.zeroone.star.basedata.entity;

import java.util.Date;


public class EnumType {

    private String id;

    private String tenantId;

    private String enumKey;

    private String displayName;

    private String description;

    private Boolean isSystem;

    private String status;

    private String createdBy;

    private Date createdAt;

    private String updatedBy;

    private Date updatedAt;

    private Integer version;

    private Boolean isDeleted;
}

/**
 * 这里设计的枚举类型集合结构如下：
 * {
 *   "_id": ObjectId("..."),
 *   "tenantId": "tenant_001",           // 多租户隔离，必填
 *   "enumKey": "order_status",          // 业务唯一键，用于代码中引用
 *   "displayName": "订单状态",           // 显示名称
 *   "description": "订单的生命周期状态",   // 描述
 *   "isSystem": false,                 // 是否系统内置（保护不可删）
 *   "status": "active",                // active/archived
 *   "validationRules": {               // 扩展校验规则
 *     "min": 1,
 *     "max": 100,
 *     "regex": "^[A-Z]+$"
 *   },
 *   "createdBy": "1",                  // 创建人，填写的是租户id
 *   "createdAt": ISODate("2026-07-19T00:00:00Z"),
 *   "updatedAt": ISODate("2026-07-19T00:00:00Z"),
 *   “updatedBy": "1"                   // 同理，为租户id
 *   "version": 1                       // 乐观锁版本号
 *   “isDeleted”: false                 // 软删除标志，逻辑删除用
 * }
 */
