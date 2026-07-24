package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.*;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：号源时段（FHIR 时段语义）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("`wkf_slot`")
@Schema(description = "号源时段")
public class WkfSlot implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    @Schema(description = "主键")
    private String id;

    /**
     * 租户标识
     */
    @Schema(description = "租户标识")
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    @Schema(description = "最后更新时间")
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    @Schema(description = "创建人")
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    @Schema(description = "最后更新人")
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
    @Schema(description = "软删标记")
    private Integer isDeleted;

    /**
     * 删除时间
     */
    @Schema(description = "删除时间")
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    @Schema(description = "删除人")
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @Version
    @Schema(description = "乐观锁版本号")
    private Long version;

    /**
     * 机构标识（冗余自排班）
     */
    @Schema(description = "机构标识")
    private String organizationId;

    /**
     * 所属日程标识
     */
    @Schema(description = "所属日程标识")
    private String scheduleId;

    /**
     * 时段开始时刻
     */
    @Schema(description = "时段开始时刻")
    private LocalDateTime startTime;

    /**
     * 时段结束时刻
     */
    @Schema(description = "时段结束时刻")
    private LocalDateTime endTime;

    /**
     * 状态码（free/busy/busy-unavailable）
     */
    @Schema(description = "状态码", example = "free")
    private String statusCode;

    /**
     * 额定容量
     */
    @Schema(description = "额定容量")
    private Integer capacity;

    /**
     * 已预约计数
     */
    @Schema(description = "已预约计数")
    private Integer bookedCount;

    /**
     * 超售标记
     */
    @Schema(description = "超售标记")
    private Integer overbookedFlag;

    /**
     * 服务大类快照 JSON
     */
    @Schema(description = "服务大类快照 JSON")
    private String serviceCategoryJson;

    /**
     * 服务类型快照 JSON
     */
    @Schema(description = "服务类型快照 JSON")
    private String serviceTypeJson;

    /**
     * 专科快照 JSON
     */
    @Schema(description = "专科快照 JSON")
    private String specialtyJson;

    /**
     * 预约类型/场景快照 JSON
     */
    @Schema(description = "预约类型/场景快照 JSON")
    private String appointmentTypeJson;

    /**
     * 备注
     */
    @Schema(description = "备注")
    private String commentText;
}
