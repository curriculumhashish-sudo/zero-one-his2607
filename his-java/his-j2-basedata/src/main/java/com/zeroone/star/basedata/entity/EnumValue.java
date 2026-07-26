package com.zeroone.star.basedata.entity;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import lombok.Data;
import org.springframework.data.annotation.*;
import org.springframework.data.domain.Page;
import org.springframework.data.mongodb.core.index.CompoundIndex;
import org.springframework.data.mongodb.core.index.CompoundIndexes;
import org.springframework.data.mongodb.core.index.Indexed;
import org.springframework.data.mongodb.core.mapping.Document;
import org.springframework.data.mongodb.core.mapping.Field;

import java.util.Date;

@Data
@Document(collection = "enum_value")
@CompoundIndexes({
        // 联合唯一索引：同一个租户下，同一个枚举类型中，enumCode 必须唯一
        @CompoundIndex(name = "idx_tenant_type_code",
                def = "{'tenantId': 1, 'enumTypeId': 1, 'enumCode': 1}",
                unique = true),
        // 联合唯一索引：同一个租户下，同一个枚举类型中，enumKey 必须唯一
        @CompoundIndex(name = "idx_tenant_type_key",
                def = "{'tenantId': 1, 'enumTypeId': 1, 'enumKey': 1}",
                unique = true),
        // 查询枚举值列表时常用：租户 + 枚举类型 + 状态 + 排序
        @CompoundIndex(name = "idx_tenant_type_status_order",
                def = "{'tenantId': 1, 'enumTypeId': 1, 'status': 1, 'order': 1}"),
        // 软删除过滤索引
        @CompoundIndex(name = "idx_tenant_type_deleted",
                def = "{'tenantId': 1, 'enumTypeId': 1, 'isDeleted': 1}")
})
public class EnumValue {

    @Id
    private String id;

    @Indexed
    @Field("enum_type_id")
    private String enumTypeId;

    @Indexed
    @Field("tenant_id")
    private String tenantId;

    @Indexed
    @Field("enum_key")
    private String enumKey;

    @Indexed
    @Field("enum_code")
    private Integer enumCode;

    @Field("display")
    private String display;

    @Field("order")
    private Integer order;

    @Indexed
    @Field("status")
    private Integer status;

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


    public static EnumDTO toDTO(EnumValue enumValue) {
        EnumDTO enumDTO = new EnumDTO();
        enumDTO.setEnumTypeId(enumValue.getEnumTypeId());
        enumDTO.setStatus(enumValue.getStatus());
        enumDTO.setEnumCode(enumValue.getEnumCode());
        enumDTO.setDisplay(enumValue.getDisplay());
        enumDTO.setOrder(enumValue.getOrder());
        enumDTO.setEnumKey(enumValue.getEnumKey());
        enumDTO.setId(enumValue.getId());

        return enumDTO;
    }

    public static PageDTO<EnumDTO> toPageDTO(Page<EnumValue> enumValuePage) {
        PageDTO<EnumDTO> pageDTO = new PageDTO<>();

        pageDTO.setTotal(enumValuePage.getTotalElements());
        pageDTO.setPages((long)enumValuePage.getTotalPages());
        pageDTO.setPageIndex((long)enumValuePage.getNumber());
        pageDTO.setPageSize((long)enumValuePage.getSize());

        pageDTO.setRows(enumValuePage.getContent().stream().map(EnumValue::toDTO).toList());

        return pageDTO;
    }
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
 *   "status": 1,                // active/inactive/deleted
 *
 *   "createdBy": "user_001",
 *   "createdAt": ISODate("2026-07-19T00:00:00Z"),
 *   "updatedAt": ISODate("2026-07-19T00:00:00Z"),
 *   "updatedBy": "user_001"
 *   "version": 1                        // 乐观锁版本号
 *   “isDeleted”: false                 // 软删除标志，逻辑删除用
 * }
 */
