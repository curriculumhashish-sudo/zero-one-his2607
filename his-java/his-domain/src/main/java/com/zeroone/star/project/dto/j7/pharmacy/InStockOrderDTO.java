package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.math.BigDecimal;
import java.time.LocalDate;
import java.util.List;

/**
 * <p>
 * 描述：药房入库单数据传输对象
 * </p>
 * <p>
 * 功能说明：用于新增、编辑和提交审批入库单。包含主表的入库单号、供应商、入库日期、
 * 药房、备注以及明细列表。明细中的行合计金额由服务端自动计算，前端可忽略。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "入库单DTO")
public class InStockOrderDTO {

    @Schema(description = "入库单号（新增时可自动生成）")
    private String orderNo;

    @Schema(description = "供应商ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long supplierId;

    @Schema(description = "入库日期", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate inDate;

    @Schema(description = "入库药房/仓库ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long warehouseId;

    @Schema(description = "主表备注")
    private String remark;

    @Schema(description = "总金额（后台自动汇总，无需传递）")
    private BigDecimal totalAmount;

    @Schema(description = "入库单明细列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<InStockOrderDetailDTO> items;
}