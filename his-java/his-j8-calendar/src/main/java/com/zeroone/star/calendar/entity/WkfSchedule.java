package com.zeroone.star.calendar.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 号源日程（FHIR 排班 语义）
 * </p>
 *
 * @author j8组
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("wkf_schedule")
public class WkfSchedule implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(type = IdType.ASSIGN_UUID)
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @Version
    private Integer version;

    /**
     * 机构标识
     */
    private String organizationId;

    /**
     * 日程显示名称
     */
    private String name;

    /**
     * 业务状态（draft/active/inactive/retired）
     */
    private String statusEnum;

    /**
     * 启用标记
     */
    private Integer activeFlag;

    /**
     * 规划窗口起始时间
     */
    private LocalDateTime planningHorizonStart;

    /**
     * 规划窗口结束时间
     */
    private LocalDateTime planningHorizonEnd;

    /**
     * 服务类型概念快照 JSON
     */
    private String serviceTypeConceptJson;

    /**
     * 医疗服务引用
     */
    private String healthcareServiceId;

    /**
     * 服务大类快照 JSON
     */
    private String serviceCategoryJson;

    /**
     * 专科快照 JSON
     */
    private String specialtyJson;

    /**
     * 可用性说明（Markdown）
     */
    private String commentMarkdown;

    /**
     * 替诊医师主键（顶班 B；原医师仍保留在 Actor）
     */
    private String substitutePractitionerId;

    /**
     * 替诊原因说明
     */
    private String substituteReason;

    /**
     * 来源 T2 日排班模板主键（模板生成时写入）
     */
    private String sourceScheduleTemplateId;

    /**
     * 关联 T1 放号时段模板主键（生成或手动创建时写入）
     */
    private String slotProvisionTemplateId;
}
