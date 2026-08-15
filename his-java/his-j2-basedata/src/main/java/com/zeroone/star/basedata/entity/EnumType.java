package com.zeroone.star.basedata.entity;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO;
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
@Document(collection = "EnumType")
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
    private String enumTypeKey;

    private String displayName;

    private String description;

    private Boolean isSystem;

    private Integer status;

    // @CreatedBy
    private String createdBy;

    // @CreatedDate
    private Date createdAt;

    // @LastModifiedBy
    private String updatedBy;

    // @LastModifiedDate
    private Date updatedAt;

    @Version
    private Integer version;

    @Indexed
    private Boolean isDeleted;

    /*
    public static EnumTypeDTO toDTO(EnumType enumType) {
        if (enumType == null) {
            return null;
        }
        EnumTypeDTO enumTypeDTO = new EnumTypeDTO();

        enumTypeDTO.setId(enumType.getId());
        enumTypeDTO.setTenantId(enumType.getTenantId());
        enumTypeDTO.setEnumTypeKey(enumType.getEnumTypeKey());
        enumTypeDTO.setDisplayName(enumType.getDisplayName());
        enumTypeDTO.setDescription(enumType.getDescription());

        enumTypeDTO.setIsSystem(enumType.getIsSystem());

        enumTypeDTO.setStatus(enumType.getStatus());

        enumTypeDTO.setCreatedBy(enumType.getCreatedBy());
        enumTypeDTO.setCreatedAt(enumType.getCreatedAt());
        enumTypeDTO.setUpdatedBy(enumType.getUpdatedBy());
        enumTypeDTO.setUpdatedAt(enumType.getUpdatedAt());

        return enumTypeDTO;
    }

    public static PageDTO<EnumTypeDTO> toPageDTO(Page<EnumType> enumTypePage) {
        PageDTO<EnumTypeDTO> pageDTO = new PageDTO<>();

        pageDTO.setPageIndex((long) enumTypePage.getNumber());
        pageDTO.setPageSize((long) enumTypePage.getSize());
        pageDTO.setTotal(enumTypePage.getTotalElements());
        pageDTO.setPages((long)enumTypePage.getTotalPages());

        pageDTO.setRows(enumTypePage.getContent().stream().map(EnumType::toDTO).toList());

        return pageDTO;
    }
    */
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
 *   "status": 1,                       // active/archived
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
