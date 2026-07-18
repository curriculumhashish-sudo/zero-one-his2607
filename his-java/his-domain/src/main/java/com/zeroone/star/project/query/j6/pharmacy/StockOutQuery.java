package com.zeroone.star.project.query.j6.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

import org.springframework.format.annotation.DateTimeFormat;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：出库单列表查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j6小组-告别
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "出库单列表查询对象")
public class StockOutQuery extends PageQuery {
    /**
     * 来源库房ID（出库药库）
     */
    @Schema(description = "出库药库ID", example = "310000000000000201")
    private String sourceLocationId;

    /**
     * 审批状态
     */
    @Schema(description = "审批状态", example = "APPROVAL",
            allowableValues = {"PENDING_APPROVAL", "APPROVAL", "AGREE", "REJECT", "WITHDRAW"})
    private String statusEnum;

    /**
     * 关键字（模糊搜索出库单号/药品名称）
     */
    @Schema(description = "关键字（模糊搜索）", example = "SO20260718")
    private String keyword;

    /**
     * 申请时间-起
     */
    @Schema(description = "申请时间起", example = "2026-07-01 00:00:00")
    @DateTimeFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    private LocalDateTime applyTimeFrom;

    /**
     * 申请时间-止
     */
    @Schema(description = "申请时间止", example = "2026-07-31 23:59:59")
    @DateTimeFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    private LocalDateTime applyTimeTo;

    /**
     * 供应类型
     */
    @Schema(description = "供应类型", example = "CABINET_STOCK_OUT")
    private String typeEnum;
}
