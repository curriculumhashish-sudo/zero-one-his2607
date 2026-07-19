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

    @Schema(description = "主键ID")
    private String id;

    @Schema(description = "业务编号（编码）", example = "CG001")
    private String busNo;

    @Schema(description = "诊疗项目名称", example = "血常规检查")
    private String name;

    @Schema(description = "目录分类", example = "检查")
    private String categoryCode;

    @Schema(description = "价格(元)", example = "35.00")
    private BigDecimal price;

    @Schema(description = "服务状态编码", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "服务状态显示名", example = "启用")
    private String statusText;

    @Schema(description = "是否激活", example = "true")
    private Boolean activeFlag;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;
}
