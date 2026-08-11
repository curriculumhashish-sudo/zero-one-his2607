package com.zeroone.star.project.dto.stocktake;

import com.fasterxml.jackson.annotation.JsonAlias;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.math.BigDecimal;

/**
 * 盘点单明细保存请求行项目 DTO
 *
 * @author j7
 */
@Data
@Schema(description = "盘点单明细行项目请求参数")
public class StocktakeItemSaveDTO {

    @Schema(description = "项目 ID（关联字典表）", example = "1001")
    @Size(max = 32, message = "项目 ID 最长 32 个字符")
    @JsonAlias("drugCode")
    private String itemId;

    @Schema(description = "项目名称", example = "阿司匹林肠溶片")
    @Size(max = 255, message = "项目名称最长 255 个字符")
    @JsonAlias("drugName")
    private String itemName;

    @Schema(description = "规格", example = "100mg*30片")
    @Size(max = 255, message = "规格最长 255 个字符")
    @JsonAlias("spec")
    private String specification;

    @Schema(description = "厂家", example = "拜耳医药保健有限公司")
    @Size(max = 255, message = "厂家最长 255 个字符")
    private String manufacturer;

    @Schema(description = "批号", example = "PH2024001")
    @Size(max = 100, message = "批号最长 100 个字符")
    private String lotNumber;

    @Schema(description = "追溯码", example = "TRACE-001")
    private String traceNo;

    @Schema(description = "账面数量", example = "100.00")
    private BigDecimal bookQuantity;

    @Schema(description = "实盘数量", example = "100.00")
    @NotNull(message = "实盘数量不能为空")
    @DecimalMin(value = "0.0", inclusive = false, message = "实盘数量必须大于 0")
    @JsonAlias("quantity")
    private BigDecimal totalQuantity;

    @Schema(description = "盈亏数量（可由实盘数量-账面数量计算）", example = "-5.00")
    private BigDecimal itemQuantity;

    @Schema(description = "包装单位编码", example = "1001")
    @Size(max = 100, message = "包装单位编码最长 100 个字符")
    @JsonAlias("packagingUnit")
    private String unitCode;

    @Schema(description = "单价", example = "12.50")
    private BigDecimal price;

    @Schema(description = "行合计金额", example = "1187.50")
    private BigDecimal totalPrice;

    @Schema(description = "实盘金额（实盘数量 × 单价）", example = "1187.50")
    private BigDecimal actualAmount;

    @Schema(description = "盘点盈亏金额", example = "-62.50")
    private BigDecimal profitLossPrice;

    @Schema(description = "明细备注", example = "正常损耗")
    private String remark;
}
