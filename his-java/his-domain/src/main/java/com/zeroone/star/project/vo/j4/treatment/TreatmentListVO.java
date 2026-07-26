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

    @Schema(description = "业务编码", example = "AD00000001")
    private String busNo;

    @Schema(description = "分类编码", example = "22")
    private String categoryCode;

    @Schema(description = "分类名称", example = "检验")
    private String categoryCode_dictText;

    @Schema(description = "状态枚举值（DRAFT-草稿/ACTIVE-已启用/INACTIVE-已停用/DISCARDED-已废弃）", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "状态名称", example = "已启用")
    private String statusEnum_dictText;

    @Schema(description = "业务类型枚举值（1-检验/2-检查/3-护理/4-手术/5-其他）", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "业务类型名称", example = "检验")
    private String typeEnum_dictText;

    @Schema(description = "价格(元)", example = "35.00")
    private BigDecimal price;
}
