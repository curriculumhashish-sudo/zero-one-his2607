package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

@Data
@Schema(description = "诊疗项目列表显示对象")
public class ServiceCatalogListVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID")
    private String id;

    @Schema(description = "项目名", example = "血常规检查")
    private String name;

    @Schema(description = "状态显示名", example = "启用")
    private String statusText;

    @Schema(description = "分类", example = "检查")
    private String categoryCode;

    @Schema(description = "价格(元)", example = "35.00")
    private BigDecimal price;
}
