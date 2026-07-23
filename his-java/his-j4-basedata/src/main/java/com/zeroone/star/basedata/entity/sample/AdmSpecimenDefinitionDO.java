package com.zeroone.star.basedata.entity.sample;


import com.baomidou.mybatisplus.annotation.*;
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
    private String tenantId;

    /** 创建时间 */
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /** 创建人 */
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    /** 最后更新人 */
    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /** 软删标记 */
    @TableLogic
    @TableField("is_deleted")
    private Integer isDeleted;

    /** 删除时间 */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /** 删除人 */
    @TableField("deleted_by")
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    @TableField("version")
    private Integer version;

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

    /** 标本种类（存 SpecimenType 枚举的 code） */
    @TableField("type_enum")
    private String typeEnum;

    /** 发布状态（存 PublicationStatus 枚举的 code） */
    @TableField("status_enum")
    private String statusEnum;
}
