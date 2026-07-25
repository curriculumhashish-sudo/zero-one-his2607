package com.zeroone.star.basedata.entity.treatment;

import com.baomidou.mybatisplus.annotation.*;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

@Getter
@Setter
@TableName("wkf_activity_definition_children")
@Schema(description = "诊疗项目子项实体")
public class WkfActivityDefinitionChildDO implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    @Schema(description = "主键", example = "1890000000000000002", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @TableField("tenant_id")
    @Schema(description = "租户标识", example = "TENANT_001")
    private String tenantId;

    @TableField(value = "created_at", fill = FieldFill.INSERT)
    @Schema(description = "创建时间", example = "2026-01-01 12:00:00")
    private LocalDateTime createdAt;

    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    @Schema(description = "最后更新时间", example = "2026-01-01 12:00:00")
    private LocalDateTime updatedAt;

    @TableField("created_by")
    @Schema(description = "创建人", example = "admin")
    private String createdBy;

    @TableField("updated_by")
    @Schema(description = "最后更新人", example = "admin")
    private String updatedBy;

    @TableLogic
    @TableField("is_deleted")
    @Schema(description = "软删标记 0-未删除 1-已删除", example = "0")
    private Integer isDeleted;

    @TableField("deleted_at")
    @Schema(description = "删除时间", example = "2026-01-01 12:00:00")
    private LocalDateTime deletedAt;

    @TableField("deleted_by")
    @Schema(description = "删除人", example = "admin")
    private String deletedBy;

    @Version
    @TableField("version")
    @Schema(description = "乐观锁版本号", example = "0")
    private Integer version;

    @TableField("activity_id")
    @Schema(description = "父项标识（外键：wkf_activity_definition.id）", example = "1890000000000000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String activityId;

    @TableField("children_id")
    @Schema(description = "子项诊疗项目标识（被引用项目主键）", example = "1890000000000000003", requiredMode = Schema.RequiredMode.REQUIRED)
    private String childrenId;

    @TableField("children_num")
    @Schema(description = "子项数量", example = "1")
    private Integer childrenNum;

    @TableField("children_name")
    @Schema(description = "子项名称快照", example = "静脉注射")
    private String childrenName;
}
