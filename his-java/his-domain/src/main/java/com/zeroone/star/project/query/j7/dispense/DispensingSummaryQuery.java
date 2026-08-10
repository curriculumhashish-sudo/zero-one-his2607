package com.zeroone.star.project.query.j7.dispense;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.*;

/**
 * <p>
 * 描述：住院发药汇总单查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zg
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "住院发药-发药汇总查询对象")
public class DispensingSummaryQuery extends PageQuery {

    @Schema(description = "汇总单号（测试库暂无住院发药汇总单数据）", example = "HZ20240719001")
    private String summaryNo;

    @Schema(description = "病区ID", example = "322955305339588608")
    private String wardId;

    @Schema(description = "病区名称", example = "内科疗区")
    private String wardName;

    @Schema(description = "状态: PENDING-待发药, COMPLETED-已发药, REFUNDED-已退药, DECLINED-已拒绝", example = "PENDING")
    private String status;

    @Schema(description = "创建时间-开始", example = "2026-06-30 00:00:00")
    private String createTimeStart;

    @Schema(description = "创建时间-结束", example = "2026-06-30 23:59:59")
    private String createTimeEnd;
}
