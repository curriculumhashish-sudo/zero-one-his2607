package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

@Getter
@Setter
@Schema(description = "诊疗项目列表数据")
public class TreatmentDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID", example = "1890000000000000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务编号（编码）", example = "AD00000001")
    private String busNo;

    @Schema(description = "诊疗项目名称", example = "肝功13项")
    private String name;

    @Schema(description = "诊疗分类编码", example = "22")
    private String categoryCode;

    @Schema(description = "价格（元）", example = "44.40")
    private BigDecimal price;

    @Schema(description = "服务状态编码（ACTIVE-启用/INACTIVE-停用）", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "服务状态显示名", example = "启用")
    private String statusText;

    @Schema(description = "是否激活", example = "true")
    private Boolean activeFlag;

    @Schema(description = "创建时间", example = "2026-01-01 12:00:00")
    private LocalDateTime createdAt;
}
