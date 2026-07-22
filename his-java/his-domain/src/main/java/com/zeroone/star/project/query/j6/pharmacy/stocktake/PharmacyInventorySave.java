package com.zeroone.star.project.query.j6.pharmacy.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述： 新增单据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单列表展示数据")
public class PharmacyInventorySave {

    @Schema(description = "盘点单主键ID")
    private Long id;

    @Schema(description = "单据号", example = "CST2026-07-190003")
    private String stocktID;

    @Schema(description = "单据状态", example = "编辑中")
    private String status;

    @Schema(description = "盘点药库名称", example = "西药库")
    private String pharmacyName;

    @Schema(description = "制单人")
    private String creator;

    @Schema(description = "制单时间")
    private LocalDateTime createTime;

    @Schema(description = "总金额")
    private BigDecimal totalAmount;
}