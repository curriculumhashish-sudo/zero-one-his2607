package com.zeroone.star.project.vo.j6.pharmacy.inbound;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(name = "InboundReceiptDetailVO", description = "药库进货单详情")
public class InboundReceiptDetailVO {

    @Schema(description = "业务单号")
    private String busNo;

    @Schema(description = "来源单号")
    private String originalBusNo;

    @Schema(description = "类型")
    private String typeEnum;

    @Schema(description = "状态")
    private String statusEnum;

    @Schema(description = "供应商ID")
    private String supplierId;

    @Schema(description = "出库方库房ID")
    private String sourceLocationId;

    @Schema(description = "入库方库房ID")
    private String purposeLocationId;

    @Schema(description = "出库方类型")
    private String sourceTypeEnum;

    @Schema(description = "入库方类型")
    private String purposeTypeEnum;

    @Schema(description = "类别")
    private String categoryEnum;

    @Schema(description = "版本号")
    private Integer rowVersion;

    @Schema(description = "申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "原因")
    private String reason;

    @Schema(description = "明细行列表")
    private List<InboundReceiptDetailLineVO> lines;

    @Data
    @Schema(name = "InboundReceiptDetailLineVO", description = "药库进货单详情明细行")
    public static class InboundReceiptDetailLineVO {
        @Schema(description = "明细主键", example = "510000000000000001")
        private String id;

        @Schema(description = "项目物理表", example = "med_medication_definition")
        private String itemTable;

        @Schema(description = "项目定义主键", example = "420000000000000001")
        private String itemId;

        @Schema(description = "行数量", example = "20.00")
        private BigDecimal itemQuantity;

        @Schema(description = "单位编码", example = "BOX")
        private String unitCode;

        @Schema(description = "单位显示文本", example = "盒")
        private String unitCodeDictText;

        @Schema(description = "批号", example = "LOT2026071901")
        private String lotNumber;

        @Schema(description = "追踪编号/追溯码", example = "TRACE202607190001")
        private String traceNo;

        @Schema(description = "单价", example = "36.500000")
        private BigDecimal price;

        @Schema(description = "合计价格", example = "730.000000")
        private BigDecimal totalPrice;

        @Schema(description = "发生时间", example = "2026-07-19T10:30:00")
        private LocalDateTime occurrenceTime;

        @Schema(description = "效期起点", example = "2026-07-01T00:00:00")
        private LocalDateTime startTime;

        @Schema(description = "效期止点", example = "2028-07-01T00:00:00")
        private LocalDateTime endTime;

        @Schema(description = "备注", example = "按订货单到货")
        private String remake;

        @Schema(description = "项目名称", example = "阿莫西林胶囊")
        private String productName;

        @Schema(description = "规格", example = "0.25g*24粒")
        private String specification;

        @Schema(description = "生产厂家", example = "广州白云山医药集团")
        private String manufacturer;
    }
}
