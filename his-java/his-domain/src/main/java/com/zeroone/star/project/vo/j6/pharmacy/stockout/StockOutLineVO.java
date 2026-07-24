package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 出库单行明细 VO
 * @author wanfengzhiyunyi
 */

@Data
@Schema(description = "出库单行明细 VO")
public class StockOutLineVO {

    @Schema(description = "主键", example = "1001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String id;

    @Schema(description = "项目表", example = "drug_info", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String itemTable;

    @Schema(description = "项目ID", example = "ITEM-5001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String itemId;

    @Schema(description = "产品名称", example = "阿莫西林胶囊", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String productName;

    @Schema(description = "规格", example = "0.5g*24粒", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String specification;

    @Schema(description = "生产厂家", example = "华北制药", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String manufacturer;

    @Schema(description = "项目数量", example = "100", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer itemQuantity;

    @Schema(description = "单位代码", example = "BOX", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String unitCode;

    @Schema(description = "单位代码描述", example = "盒", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String unitCodeDictText;

    @Schema(description = "批号", example = "B20240701", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String lotNumber;

    @Schema(description = "开始时间", example = "2024-07-19 00:00:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String startTime;

    @Schema(description = "结束时间", example = "2024-07-19 23:59:59", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String endTime;

    @Schema(description = "追溯编号", example = "TR20240719001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String traceNo;

    @Schema(description = "备注", example = "加急处理", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String remake;

    @Schema(description = "价格", example = "12.50", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer price;

    @Schema(description = "总价", example = "1250.00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer totalPrice;

    @Schema(description = "发生时间", example = "2024-07-19 10:30:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String occurrenceTime;

    @Schema(description = "原因", example = "门诊发药", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String reason;







}
