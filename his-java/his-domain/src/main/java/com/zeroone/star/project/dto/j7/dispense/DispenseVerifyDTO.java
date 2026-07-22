package com.zeroone.star.project.dto.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

/**
 * <p>
 * 描述：核对发药请求传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zg
 * @version 1.0.0
 */
@Schema(description = "核对发药请求数据")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class DispenseVerifyDTO {

    @Schema(description = "汇总单号", example = "HZ20240719001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String summaryNo;

    @Schema(description = "发药明细核对列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<VerifyItem> items;

    /**
     * 单条发药核对明细
     */
    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    @Schema(description = "发药核对明细项")
    public static class VerifyItem {

        @Schema(description = "发药记录主键", example = "d12345678901234567890", requiredMode = Schema.RequiredMode.REQUIRED)
        private String dispenseId;

        @Schema(description = "实发数量", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
        private java.math.BigDecimal dispenseQuantity;

        @Schema(description = "批号", example = "BATCH20240701")
        private String lotNumber;
    }
}
