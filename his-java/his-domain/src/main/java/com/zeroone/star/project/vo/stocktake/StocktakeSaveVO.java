package com.zeroone.star.project.vo.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

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

    @Schema(description = "业务单号", example = "PST202607300001")
    private String busNo;

    @Schema(description = "状态枚举", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "明细条数", example = "3")
    private Integer itemCount;

    @Schema(description = "总金额", example = "1187.50")
    private BigDecimal totalPrice;

    @Schema(description = "操作时间", example = "2026-07-30 10:00:00")
    private LocalDateTime operateTime;
}
