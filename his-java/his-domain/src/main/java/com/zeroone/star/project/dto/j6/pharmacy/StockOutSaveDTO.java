package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 描述：出库单新增/保存/保存并提交DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j6小组-告别
 * @version 1.0.0
 */
@Data
@Schema(description = "出库单保存对象")
public class StockOutSaveDTO {
    /**
     * 业务单号（编辑时必填，新增时为空）
     */
    @Schema(description = "业务单号（编辑时必填）", example = "SO202607180001")
    private String busNo;

    /**
     * 来源业务单号
     */
    @Schema(description = "来源业务单号", example = "PO202607150001")
    private String originalBusNo;

    /**
     * 来源库房ID（出库药库）
     */
    @Schema(description = "出库药库ID", example = "310000000000000201", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "出库药库不能为空")
    private String sourceLocationId;

    /**
     * 目的库房ID（目的药房）
     */
    @Schema(description = "目的药房ID", example = "310000000000000301", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "目的药房不能为空")
    private String purposeLocationId;

    /**
     * 操作人员ID
     */
    @Schema(description = "操作人员ID", example = "310000000000000101")
    private String practitionerId;

    /**
     * 行版本号（乐观锁，编辑时必填）
     */
    @Schema(description = "行版本号（乐观锁）", example = "0")
    private Integer rowVersion;

    /**
     * 出库单明细行列表
     */
    @Schema(description = "出库单明细行列表")
    @NotEmpty(message = "明细行不能为空")
    @Valid
    private List<StockOutLineDTO> lines;

    /**
     * <p>
     * 描述：出库单明细行
     * </p>
     */
    @Data
    @Schema(description = "出库单明细行")
    public static class StockOutLineDTO {
        /**
         * 物品定义表名
         */
        @Schema(description = "物品定义表名", example = "med_medication_definition", requiredMode = Schema.RequiredMode.REQUIRED)
        @NotBlank(message = "物品定义表名不能为空")
        private String itemTable;

        /**
         * 物品ID
         */
        @Schema(description = "物品ID（药品/耗材ID）", example = "310000000000000401", requiredMode = Schema.RequiredMode.REQUIRED)
        @NotBlank(message = "物品ID不能为空")
        private String itemId;

        /**
         * 出库数量
         */
        @Schema(description = "出库数量", example = "100.00", requiredMode = Schema.RequiredMode.REQUIRED)
        @NotNull(message = "出库数量不能为空")
        private BigDecimal itemQuantity;

        /**
         * 计量单位编码
         */
        @Schema(description = "计量单位编码", example = "BOX", requiredMode = Schema.RequiredMode.REQUIRED)
        @NotBlank(message = "计量单位不能为空")
        private String unitCode;

        /**
         * 批号
         */
        @Schema(description = "批号", example = "B202607001")
        private String lotNumber;

        /**
         * 库存项ID
         */
        @Schema(description = "库存项ID", example = "310000000000000501")
        private String inventoryItemId;

        /**
         * 追溯码
         */
        @Schema(description = "追溯码", example = "TRC202607180001")
        private String traceNo;

        /**
         * 单价
         */
        @Schema(description = "单价", example = "15.50")
        private BigDecimal price;

        /**
         * 合计金额
         */
        @Schema(description = "合计金额", example = "1550.00")
        private BigDecimal totalPrice;

        /**
         * 发生时间
         */
        @Schema(description = "发生时间", example = "2026-07-18 09:00:00")
        private LocalDateTime occurrenceTime;

        /**
         * 原因
         */
        @Schema(description = "原因", example = "科室申领")
        private String reason;

        /**
         * 备注
         */
        @Schema(description = "备注", example = "加急出库")
        private String remake;
    }
}
