package com.zeroone.star.project.vo.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 描述：盘点单保存结果
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单保存结果")
public class StocktakeSaveVO {

    @Schema(description = "盘点单ID", example = "330268165805182976")
    private String id;

    @Schema(description = "业务单号", example = "PST202607300001")
    private String busNo;

    @Schema(description = "状态枚举", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "制单人标识", example = "308791351298756608")
    private String createdBy;

    @Schema(description = "制单人名称", example = "张三")
    private String createdByName;

    @Schema(description = "制单时间", example = "2026-08-05 08:00:00")
    private LocalDateTime createdAt;

    @Schema(description = "审批人标识", example = "308791351298756608")
    private String approverId;

    @Schema(description = "审批人名称", example = "李四")
    private String approverName;

    @Schema(description = "审批时间", example = "2026-08-05 10:00:00")
    private LocalDateTime approvalTime;

    @Schema(description = "盘点药房标识", example = "322954949549363200")
    private String warehouseId;

    @Schema(description = "盘点药房名称", example = "西药房")
    private String warehouseName;

    @Schema(description = "盘点药房名称", example = "西药房")
    private String stocktakePharmacy;

    @Schema(description = "明细条数", example = "3")
    private Integer itemCount;

    @Schema(description = "总金额", example = "1187.50")
    private BigDecimal totalPrice;

    @Schema(description = "操作时间", example = "2026-07-30 10:00:00")
    private LocalDateTime operateTime;

    @Schema(description = "盘点单明细列表；新增单据时返回空数组")
    private List<StocktakeItemVO> items;
}
