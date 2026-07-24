package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

@Data
@Schema(description = "诊疗项目选择显示对象（其他业务选择诊疗时使用）")
public class TreatmentSelectVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键(编码)", example = "123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "项目名称", example = "血常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "诊疗分类", example = "CHECK", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;
}
