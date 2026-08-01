package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.*;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 对应表：wkf_schedule_actor
 * </p>
 *
 * @author www
 */
@Data
@ToString
@TableName("wkf_schedule_actor")
public class ScheduleActor implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @Schema(description = "主键")
    @TableId(value = "id", type = IdType.INPUT)
    private String id;
    /**
     * 租户标识
     */
    @Schema(description = "租户标识")
    @TableField("tenant_id")
    private String tenantId;
    /**
     * 创建时间
     */
    @Schema(description = "创建时间")
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;
    /**
     * 最后更新时间
     */
    @Schema(description = "最后更新时间")
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;
    /**
     * 创建人
     */
    @Schema(description = "创建人")
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;
    /**
     * 最后更新人
     */
    @Schema(description = "最后更新人")
    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;
    /**
     * 软删标记
     */
    @Schema(description = "软删标记")
    @TableField("is_deleted")
    private Boolean isDeleted;
    /**
     * 删除时间
     */
    @Schema(description = "删除时间")
    @TableField(value = "deleted_at", fill = FieldFill.UPDATE)
    private LocalDateTime deletedAt;
    /**
     * 删除人
     */
    @Schema(description = "删除人")
    @TableField(value = "deleted_by", fill = FieldFill.UPDATE)
    private String deletedBy;
    /**
     * 乐观锁版本号
     */
    @Schema(description = "乐观锁版本号")
    @TableField("version")
    private Integer version;
    /**
     * 所属日程标识
     */
    @Schema(description = "所属日程标识")
    @TableField("schedule_id")
    private String scheduleId;
    /**
     * 参与者类型编码
     */
    @Schema(description = "参与者类型编码")
    @TableField("actor_type")
    private Integer actorType;
    /**
     * 参与者主键
     */
    @Schema(description = "参与者主键")
    @TableField("actor_id")
    private String actorId;
    /**
     * 排序号
     */
    @Schema(description = "排序号")
    @TableField("sort_no")
    private Integer sortNo;


}