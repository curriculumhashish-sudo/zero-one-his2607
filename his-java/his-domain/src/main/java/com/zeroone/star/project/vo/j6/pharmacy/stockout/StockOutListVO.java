package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@Schema(description = "出库单列表行")
public class StockOutListVO {

    @Schema(description = "业务单号", example = "CSO2026-07-190004", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "单据类型枚举", example = "CABINET_STOCK_OUT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "状态枚举", example = "AGREE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "状态文本", example = "同意", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnumText;

    @Schema(description = "源药库ID", example = "3229548576228024232", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sourceLocationId;

    @Schema(description = "源药库名称", example = "门诊西药房", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sourceLocationName;

    @Schema(description = "目的药房ID", example = "3229549549549363200", requiredMode = Schema.RequiredMode.REQUIRED)
    private String purposeLocationId;

    @Schema(description = "目的药房名称", example = "门诊西药房", requiredMode = Schema.RequiredMode.REQUIRED)
    private String purposeLocationName;

    @Schema(description = "申请时间", example = "2026-07-19 18:35:38", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime applyTime;

    @Schema(description = "审批时间", example = "2026-07-19 18:35:42", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDateTime approvalTime;

    @Schema(description = "发生时间", example = "2026-07-19 18:35:22", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime occurrenceTime;

    @Schema(description = "申请人ID", example = "380791351297856608", requiredMode = Schema.RequiredMode.REQUIRED)
    private String applicantId;

    @Schema(description = "申请人名称", example = "admin", requiredMode = Schema.RequiredMode.REQUIRED)
    private String creatorName;

    @Schema(description = "审批人ID", example = "380791351297856608", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String approverId;

    @Schema(description = "审批人名称", example = "admin", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String approverName;

    @Schema(description = "总金额", example = "6.990000", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal totalAmount;

    @Schema(description = "原业务单号", example = "null", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String originalBusNo;

    @Schema(description = "供应商ID", example = "SUP-1001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String supplierId;

    @Schema(description = "物品ID", example = "ITEM-5001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String itemId;

    @Schema(description = "物品表名", example = "drug_info", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String itemTable;

    @Schema(description = "物品数量", example = "100", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer itemQuantity;

    @Schema(description = "单位编码", example = "BOX", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String unitCode;

    @Schema(description = "批号", example = "B20240701", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String lotNumber;

    @Schema(description = "追溯号", example = "TR20240719001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String traceNo;

    @Schema(description = "单价", example = "12.50", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer price;

    @Schema(description = "总价", example = "1250.00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer totalPrice;

    @Schema(description = "项目名称", example = "阿莫西林胶囊", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "规格", example = "0.25g*24粒", requiredMode = Schema.RequiredMode.REQUIRED)
    private String specification;

    @Schema(description = "厂家/产地", example = "华北制药股份有限公司", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String manufacturer;

    @Schema(description = "备注", example = "需冷藏保存", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String remark;

}
