package com.zeroone.star.project.vo.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "修改器材响应")
public class EquipmentUpdateVO {

    @Schema(description = "器材业务编码（院内唯一）", example = "DEV-2026-0015926", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "器材名称", example = "一次性无菌注射器", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "规格型号", example = "5ml/支")
    private String modelNumber;

    @Schema(description = "发布状态", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "器材分类编码", example = "11")
    private String categoryCode;

    @Schema(description = "生产厂家", example = "康乐医疗器械有限公司")
    private String manufacturerText;

    @Schema(description = "零售价（元）", example = "25.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal retailPrice;

    @Schema(description = "更新时间", example = "2026-07-19 21:30:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private String updateTime;
}
