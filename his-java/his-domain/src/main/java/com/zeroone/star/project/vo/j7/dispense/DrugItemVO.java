package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "处方药品明细")
public class DrugItemVO {

    @Schema(description = "药品名称", example = "阿托伐他汀钙片")
    private String drugName;

    @Schema(description = "规格", example = "20mg*7片")
    private String spec;

    @Schema(description = "数量", example = "2")
    private BigDecimal quantity;

    @Schema(description = "单位", example = "盒")
    private String unit;
}