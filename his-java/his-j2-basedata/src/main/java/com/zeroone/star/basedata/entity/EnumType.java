package com.zeroone.star.basedata.entity;

import lombok.Data;
import org.springframework.data.annotation.*;
import org.springframework.data.mongodb.core.index.CompoundIndex;
import org.springframework.data.mongodb.core.index.CompoundIndexes;
import org.springframework.data.mongodb.core.index.Indexed;
import org.springframework.data.mongodb.core.mapping.Document;
import org.springframework.data.mongodb.core.mapping.Field;

import java.util.Date;

@Data
@Document(collection = "enum_type")
@CompoundIndexes({
        // 复合索引，确保租户下enumTypeKey唯一
        @CompoundIndex(name = "idx_tenantId_enumKey",
                def = "{'tenantId': 1, 'enumTypeKey': 1}",
                unique = true),
        // 查询时常用复合索引
        @CompoundIndex(name = "idx_tenantId_status",
                def = "{'tenantId': 1, 'status': 1}")
})
public class EnumType {

    @Id
    private String id;

    @Indexed
    private String tenantId;
    
    @Indexed
    @Field("enum_type_key")
    private String enumTypeKey;

    @Field("display_name")
    private String displayName;

    @Field("description")
    private String description;

    @Field("is_system")
    private Boolean isSystem;

    @Field("status")
    private String status;

    // @CreatedBy
    @Field("created_by")
    private String createdBy;

    // @CreatedDate
    @Field("created_at")
    private Date createdAt;

    // @LastModifiedBy
    @Field("updated_by")
    private String updatedBy;

    // @LastModifiedDate
    @Field("updated_at")
    private Date updatedAt;

    @Version
    private Integer version;

    @Indexed
    @Field("is_deleted")
    private Boolean isDeleted;
}

/**
 * 这里设计的枚举类型集合结构如下：
 * {
 *   "_id": ObjectId("..."),
 *   "tenantId": "tenant_001",           // 多租户隔离，必填
 *   "enumTypeKey": "order_status",          // 业务唯一键，用于代码中引用
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
