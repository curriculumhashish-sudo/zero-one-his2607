package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

@Getter
@Setter
@Schema(description = "诊疗项目详情数据")
public class TreatmentDetailDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID")
    private String id;

    @Schema(description = "租户标识")
    private String tenantId;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;

    @Schema(description = "诊疗项目名称", example = "血常规检查")
    private String name;

    @Schema(description = "服务类型编码", example = "OUTPATIENT_SERVICE")
    private String typeCode;

    @Schema(description = "服务类型名称", example = "门诊服务")
    private String typeName;

    @Schema(description = "目录分类", example = "检查")
    private String categoryCode;

    @Schema(description = "提供机构ID")
    private String offeredOrgId;

    @Schema(description = "专科码", example = "CARDIOLOGY")
    private String specialtyCode;

    @Schema(description = "服务地点ID")
    private String locationId;

    @Schema(description = "服务状态编码", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "服务状态显示名", example = "启用")
    private String statusText;

    @Schema(description = "是否激活可用", example = "true")
    private Boolean activeFlag;

    @Schema(description = "是否需要预约", example = "false")
    private Boolean appointmentRequiredFlag;

    @Schema(description = "医保编号")
    private String ybNo;

    @Schema(description = "业务编号（编码）", example = "CG001")
    private String busNo;

    @Schema(description = "备注描述")
    private String comment;

    @Schema(description = "扩展详情JSON")
    private String extraDetails;

    @Schema(description = "联系方式")
    private String contact;

    @Schema(description = "价格(元)", example = "35.00")
    private BigDecimal price;
}
