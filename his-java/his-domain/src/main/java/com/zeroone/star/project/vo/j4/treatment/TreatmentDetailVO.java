package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;

@Data
@Schema(description = "诊疗项目详情显示对象")
public class TreatmentDetailVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID", example = "1873645982637461")
    private String id;

    @Schema(description = "租户ID", example = "T001")
    private String tenantId;

    @Schema(description = "创建时间", example = "2026-07-22T10:30:00")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间", example = "2026-07-22T14:00:00")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人", example = "张医生")
    private String createdBy;

    @Schema(description = "更新人", example = "李护士")
    private String updatedBy;

    @Schema(description = "逻辑删除标记：0未删除/1已删除", example = "0")
    private Integer isDeleted;

    @Schema(description = "删除时间", example = "null")
    private LocalDateTime deletedAt;

    @Schema(description = "删除人", example = "null")
    private String deletedBy;

    @Schema(description = "乐观锁版本号", example = "1")
    private Integer version;

    @Schema(description = "诊疗项目名称", example = "血常规检查")
    private String name;

    @Schema(description = "服务类型编码", example = "LAB")
    private String typeCode;

    @Schema(description = "分类编码", example = "检查")
    private String categoryCode;

    @Schema(description = "提供机构ID", example = "ORG001")
    private String offeredOrgId;

    @Schema(description = "专业科室编码", example = "DEP01")
    private String specialtyCode;

    @Schema(description = "执行地点ID", example = "LOC001")
    private String locationId;

    @Schema(description = "状态枚举：ACTIVE启用/INACTIVE停用", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "是否有效标记", example = "true")
    private Boolean activeFlag;

    @Schema(description = "是否需要预约标记", example = "false")
    private Boolean appointmentRequiredFlag;

    @Schema(description = "医保编码", example = "YB1234567890")
    private String ybNo;

    @Schema(description = "业务编号（编码）", example = "XM20260722001")
    private String busNo;

    @Schema(description = "备注说明", example = "需空腹8小时")
    private String comment;

    @Schema(description = "扩展详情（JSON格式）", example = "{}")
    private String extraDetails;

    @Schema(description = "联系方式", example = "010-12345678")
    private String contact;
}
