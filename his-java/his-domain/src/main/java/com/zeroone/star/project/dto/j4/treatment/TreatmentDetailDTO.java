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

    @Schema(description = "主键ID", example = "1890000000000000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "租户标识", example = "TENANT_001")
    private String tenantId;

    @Schema(description = "创建时间", example = "2026-01-01 12:00:00")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间", example = "2026-01-01 12:00:00")
    private LocalDateTime updatedAt;

    @Schema(description = "诊疗项目名称", example = "肝功13项")
    private String name;

    @Schema(description = "服务类型编码", example = "1")
    private String typeEnum;

    @Schema(description = "服务类型名称", example = "检验")
    private String typeName;

    @Schema(description = "诊疗分类编码", example = "22")
    private String categoryCode;

    @Schema(description = "提供机构ID", example = "1001")
    private String orgId;

    @Schema(description = "专科码", example = "CARDIOLOGY")
    private String specialtyCode;

    @Schema(description = "服务地点ID", example = "2001")
    private String locationId;

    @Schema(description = "服务状态编码（ACTIVE-启用/INACTIVE-停用）", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "服务状态显示名", example = "启用")
    private String statusText;

    @Schema(description = "是否激活可用", example = "true")
    private Boolean activeFlag;

    @Schema(description = "是否需要预约", example = "false")
    private Boolean appointmentRequiredFlag;

    @Schema(description = "医保编号", example = "C010101001")
    private String ybNo;

    @Schema(description = "业务编号（编码）", example = "AD00000001")
    private String busNo;

    @Schema(description = "备注描述", example = "常规肝功能检测项目")
    private String comment;

    @Schema(description = "扩展详情JSON", example = "{\"detail\":\"肝功全套\"}")
    private String extraDetails;

    @Schema(description = "联系方式", example = "{\"phone\":\"010-12345678\"}")
    private String contact;

    @Schema(description = "价格（元）", example = "44.40")
    private BigDecimal price;
}
