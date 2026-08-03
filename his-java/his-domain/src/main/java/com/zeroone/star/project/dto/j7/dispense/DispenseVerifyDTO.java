package com.zeroone.star.project.dto.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
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

    @NotBlank(message = "汇总单号不能为空")
    @Size(max = 255, message = "汇总单号长度不能超过255个字符")
    @Schema(description = "汇总单号", example = "HZ20240719001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String summaryNo;

    @NotEmpty(message = "发药明细不能为空")
    @Valid
    @Schema(description = "发药明细核对列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<VerifyItem> items;

    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    @Schema(description = "发药核对明细项")
    public static class VerifyItem {

        @NotBlank(message = "发药记录主键不能为空")
        @Size(max = 64, message = "发药记录主键长度不能超过64个字符")
        @Schema(description = "发药记录主键", example = "d12345678901234567890", requiredMode = Schema.RequiredMode.REQUIRED)
        private String dispenseId;

        @NotNull(message = "实发数量不能为空")
        @Schema(description = "实发数量", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
        private java.math.BigDecimal dispenseQuantity;

        @Size(max = 255, message = "批号长度不能超过255个字符")
        @Schema(description = "批号", example = "BATCH20240701")
        private String lotNumber;
    }
}
