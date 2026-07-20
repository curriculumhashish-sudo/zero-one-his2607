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
 * 药库进货单新增、保存、保存并提交数据对象。
 */
@Data
@Schema(name = "InboundReceiptSaveDTO", description = "药库进货单新增/保存/保存并提交请求")
public class InboundReceiptSaveDTO {
    @Schema(description = "业务单号，新增时可为空，保存或提交时传入", example = "JHD202607190001")
    private String busNo;

    @Schema(description = "来源业务单号，如关联药库订货单号", example = "DHD202607180001")
    private String originalBusNo;

    @NotBlank(message = "供应类型不能为空")
    @Schema(description = "供应类型编码", example = "INBOUND")
    private String typeEnum;

    @NotBlank(message = "供应类别不能为空")
    @Schema(description = "供应类别编码，采购入库通常为 PURCHASE", example = "PURCHASE")
    private String categoryEnum;

    @Schema(description = "来源位置类型，药库进货单可为空", example = "SUPPLIER")
    private String sourceTypeEnum;

    @Schema(description = "来源位置标识，药库进货单可为空", example = "4100000000000000008")
    private String sourceLocationId;

    @NotBlank(message = "目的位置类型不能为空")
    @Schema(description = "目的位置类型", example = "WAREHOUSE")
    private String purposeTypeEnum;

    @NotBlank(message = "目的库房不能为空")
    @Schema(description = "目的库房标识", example = "322954885380706304")
    private String purposeLocationId;

    @NotBlank(message = "供应商不能为空")
    @Schema(description = "供应商标识", example = "4100000000000000008")
    private String supplierId;

    @Schema(description = "经办人员标识", example = "100000000000000001")
    private String practitionerId;

    @Schema(description = "原因/备注原因", example = "按药库订货单到货")
    private String reason;

    @Schema(description = "乐观锁版本号，保存或提交已有单据时传入", example = "0")
    private Integer version;

    @Valid
    @NotEmpty(message = "进货明细不能为空")
    @Schema(description = "进货明细")
    private List<InboundReceiptLineDTO> lines;

    @Data
    @Schema(name = "InboundReceiptLineDTO", description = "药库进货单明细行")
    public static class InboundReceiptLineDTO {
        @NotBlank(message = "项目物理表不能为空")
        @Schema(description = "项目物理表", example = "med_medication_definition")
        private String itemTable;

        @NotBlank(message = "项目标识不能为空")
        @Schema(description = "项目定义主键", example = "420000000000000001")
        private String itemId;

        @NotNull(message = "进货数量不能为空")
        @Schema(description = "行数量", example = "20.00")
        private BigDecimal itemQuantity;

        @NotBlank(message = "单位编码不能为空")
        @Schema(description = "单位编码", example = "BOX")
        private String unitCode;

        @Schema(description = "批号", example = "LOT2026071901")
        private String lotNumber;

        @Schema(description = "追踪编号/追溯码，多个可按业务约定分隔", example = "TRACE202607190001")
        private String traceNo;

        @Schema(description = "单价", example = "36.500000")
        private BigDecimal price;

        @Schema(description = "合计价格", example = "730.000000")
        private BigDecimal totalPrice;

        @Schema(description = "发生时间", example = "2026-07-19T10:30:00")
        private LocalDateTime occurrenceTime;

        @Schema(description = "效期区间起点", example = "2026-07-01T00:00:00")
        private LocalDateTime startTime;

        @Schema(description = "效期区间止点", example = "2028-07-01T00:00:00")
        private LocalDateTime endTime;

        @Schema(description = "行备注", example = "按订货单到货")
        private String remake;
    }
}
