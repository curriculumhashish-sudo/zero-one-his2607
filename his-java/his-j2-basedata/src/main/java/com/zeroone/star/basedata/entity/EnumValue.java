package com.zeroone.star.basedata.entity;

import java.util.Date;

public class EnumValue {

    private String id;

    private String enumTypeId;

    private String tenantId;

    private String enumKey;

    private String enumCode;

    private String display;

    private Integer order;

    private String status;

    private String createdBy;

    private Date createdAt;

    private String updatedBy;

    private Date updatedAt;

    private Integer version;

    private Boolean isDeleted;
}

/**
 * 相较于字典系统，枚举字典更倾向于后端，个人认为其不用添加描述来解释
 * {
 *   "_id": ObjectId("..."),
 *   "enumTypeId": ObjectId("..."),    // 关联 enum_type._id
 *   "tenantId": "tenant_001",           // 冗余字段，便于按租户分片
 *   "enumKey": "SHIPPED",                 // 存储字符
 *   "enumCode": 1                             // 枚举对应的实数值，一个枚举类型的枚举值的code，string互不相同
 *   "display": "已发货",                  // 显示文本（支持多语言）
 *   "order": 10,                        // 排序权重，数值越小越靠前
 *   "status": "active",                // active/inactive/deleted
 *
 *   "createdBy": "user_001",
 *   "createdAt": ISODate("2026-07-19T00:00:00Z"),
 *   "updatedAt": ISODate("2026-07-19T00:00:00Z"),
 *   "updatedBy": "user_001"
 *   "version": 1                        // 乐观锁版本号
 *   “isDeleted”: false                 // 软删除标志，逻辑删除用
 * }
 */
