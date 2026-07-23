package com.zeroone.star.project.vo.j7.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：新增盘点单草稿返回对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "新增盘点单草稿返回对象")
public class StocktakeDraftVO {

    @Schema(description = "前端临时草稿ID", example = "draft-202607230001")
    private String draftId;

    @Schema(description = "默认状态编码", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "默认状态名称", example = "编辑中")
    private String statusName;

    @Schema(description = "默认供应类别", example = "STOCK_SUPPLY")
    private String categoryEnum;

    @Schema(description = "创建草稿时间", example = "2026-07-23T10:30:00")
    private LocalDateTime draftTime;
}
