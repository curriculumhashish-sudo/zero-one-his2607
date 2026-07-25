package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

@Data
@Schema(description = "诊疗项目列表显示对象")
public class TreatmentListVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键(编码)", example = "123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "项目名称", example = "血常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "发布状态枚举编码", example = "PUBLISH", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "诊疗类型枚举编码", example = "CHECK", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "价格(元)", example = "35.00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private BigDecimal price;
}
