package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.List;

/**
 * <p>
 * 描述：药房入库单数据传输对象
 * </p>
 * <p>
 * 功能说明：用于新增、编辑和提交审批入库单。包含主表信息及药品明细列表。
 * 整单总金额由服务端自动汇总明细行的合计金额，前端无需传递。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@Schema(description = "入库单DTO")
public class InStockOrderDTO implements Serializable {

    @Schema(description = "入库单号（业务号），新增时可自动生成")
    private String orderNo;

    @Schema(description = "供应商ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long supplierId;

    @Schema(description = "入库日期", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate inDate;

    @Schema(description = "入库药房/仓库ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long warehouseId;

    @Schema(description = "主表备注")
    private String remark;

    @Schema(description = "入库单明细列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<InStockOrderDetailDTO> items;
}