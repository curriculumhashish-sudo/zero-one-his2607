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
 * 描述：号源日程（FHIR 排班语义）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("`wkf_schedule`")
@Schema(description = "号源日程")
public class WkfSchedule implements Serializable {

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
    private Integer version;

    /**
     * 机构标识
     */
    @Schema(description = "机构标识")
    private String organizationId;

    /**
     * 日程显示名称
     */
    @Schema(description = "日程显示名称")
    private String name;

    /**
     * 业务状态（draft/active/inactive/retired）
     */
    @Schema(description = "业务状态", example = "active")
    private String statusEnum;

    /**
     * 启用标记
     */
    @Schema(description = "启用标记")
    private Integer activeFlag;

    /**
     * 规划窗口起始时间
     */
    @Schema(description = "规划窗口起始时间")
    private LocalDateTime planningHorizonStart;

    /**
     * 规划窗口结束时间
     */
    @Schema(description = "规划窗口结束时间")
    private LocalDateTime planningHorizonEnd;

    /**
     * 服务类型概念快照 JSON
     */
    @Schema(description = "服务类型概念快照 JSON")
    private String serviceTypeConceptJson;

    /**
     * 医疗服务引用
     */
    @Schema(description = "医疗服务引用")
    private String healthcareServiceId;

    /**
     * 服务大类快照 JSON
     */
    @Schema(description = "服务大类快照 JSON")
    private String serviceCategoryJson;

    /**
     * 专科快照 JSON
     */
    @Schema(description = "专科快照 JSON")
    private String specialtyJson;

    /**
     * 可用性说明（Markdown）
     */
    @Schema(description = "可用性说明（Markdown）")
    private String commentMarkdown;

    /**
     * 替诊医师主键
     */
    @Schema(description = "替诊医师主键")
    private String substitutePractitionerId;

    /**
     * 替诊原因说明
     */
    @Schema(description = "替诊原因说明")
    private String substituteReason;

    /**
     * 来源 T2 日排班模板主键
     */
    @Schema(description = "来源 T2 日排班模板主键")
    private String sourceScheduleTemplateId;

    /**
     * 关联 T1 放号时段模板主键
     */
    @Schema(description = "关联 T1 放号时段模板主键")
    private String slotProvisionTemplateId;
}
