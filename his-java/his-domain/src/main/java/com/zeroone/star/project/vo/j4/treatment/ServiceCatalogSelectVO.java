package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

@Data
@Schema(description = "诊疗项目选择显示对象（其他业务选择诊疗时使用）")
public class ServiceCatalogSelectVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID")
    private String id;

    @Schema(description = "编码", example = "CG001")
    private String busNo;

    @Schema(description = "项目名", example = "血常规检查")
    private String name;

    @Schema(description = "分类", example = "检查")
    private String categoryCode;
}
