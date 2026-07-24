package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 对应表：wkf_schedule
 * </p>
 *
 * @author www
 */
@Data
@ToString
@TableName("wkf_schedule")
public class Schedule implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键")
    @TableId(value = "id", type = IdType.INPUT)
    private Long id;

    @Schema(description = "租户标识")
    private Long tenantId;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人")
    private Long createdBy;

    @Schema(description = "最后更新人")
    private Long updatedBy;

    @Schema(description = "软删标记")
    private Boolean isDeleted;

    @Schema(description = "删除时间")
    private LocalDateTime deletedAt;

    @Schema(description = "删除人")
    private Long deletedBy;

    @Schema(description = "乐观锁版本号")
    private Integer version;

    @Schema(description = "机构标识")
    private Long organizationId;

    @Schema(description = "日程显示名称")
    private String name;

    @Schema(description = "业务状态")
    private String statusEnum;

    @Schema(description = "启用标记")
    private Boolean activeFlag;

    @Schema(description = "规划窗口起始时间")
    private LocalDateTime planningHorizonStart;

    @Schema(description = "规划窗口结束时间")
    private LocalDateTime planningHorizonEnd;

    @Schema(description = "服务类型概念快照 JSON")
    private String serviceTypeConceptJson;

    @Schema(description = "医疗服务引用")
    private Long healthcareServiceId;

    @Schema(description = "服务大类快照 JSON")
    private String serviceCategoryJson;

    @Schema(description = "专科快照 JSON")
    private String specialtyJson;

    @Schema(description = "可用性说明（Markdown）")
    private String commentMarkdown;

    @Schema(description = "替诊医师主键（顶班 B；原医师仍保留在 Actor）")
    private Long substitutePractitionerId;

    @Schema(description = "替诊原因说明")
    private String substituteReason;


}