package com.zeroone.star.basedata.entity.sample;


import com.baomidou.mybatisplus.annotation.*;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：样本管理实体（对应表 adm_specimen_definition）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@TableName("adm_specimen_definition")
public class AdmSpecimenDefinitionDO {

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    @TableField("tenant_id")
    @Schema(description = "租户标识")
    private String tenantId;

    /** 创建时间（insert 时由 MetaObjectHandler 自动填充） */
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;


    /** 最后更新时间 */
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /** 创建人（insert 时由 MetaObjectHandler 自动填充） */
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    /** 最后更新人（insert/update 时由 MetaObjectHandler 自动填充） */
    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记（0=未删除, 1=已删除）
     * 默认 0，插入时无需业务层显式赋值
     */
    @TableLogic
    @TableField("is_deleted")
    private Integer isDeleted = 0;

    /**
     * 删除时间
     * 注意：@TableLogic 软删只更新 is_deleted，不会自动填充本字段，
     * 需业务层在删除前 update 设置（或扩展 MetaObjectHandler/拦截器）
     */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /**
     * 删除人
     * 注意：同上，软删时不会自动填充，需业务层配合设置
     */
    @TableField("deleted_by")
    private String deletedBy;

    /** 乐观锁版本号（默认 0，更新时由 OptimisticLockerInnerInterceptor 自动 +1） */
    @Version
    @TableField("version")
    private Integer version = 0;

    /** 样本定义聚合根（业务编码） */
    @TableField("bus_no")
    private String busNo;

    /** 面向临床与维护页的显示名称 */
    @TableField("name")
    private String name;

    /** 拼音检索码 */
    @TableField("py_str")
    private String pyStr;

    /** 五笔检索码 */
    @TableField("wb_str")
    private String wbStr;

    /** 标本种类（存 SpecimenType 枚举的 code，取值见 SampleConstants.TYPE_MAP） */
    @TableField("type_enum")
    private String typeEnum;

    /** 发布状态（存 PublicationStatus 枚举的 code，取值见 SampleConstants.STATUS_MAP） */
    @TableField("status_enum")
    private String statusEnum;
}
