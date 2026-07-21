package com.zeroone.star.project.vo.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：汇总单视图对象
 * </p>
 * <p>
 * 功能说明：用于展示发药汇总单信息，包含汇总单编号、名称、状态等。
 * 数据来源于 summary_order 表。
 * 在获取汇总单队列时，将数据库记录转换为该 VO 返回给前端。
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
@Schema(description = "汇总单VO")
public class SummaryVO {
    @Schema(description = "汇总单ID")
    private String summaryId;

    @Schema(description = "汇总单编号")
    private String summaryCode;

    @Schema(description = "汇总单名称")
    private String summaryName;

    @Schema(description = "状态（0-待发药 1-已发药 2-已驳回）")
    private Integer status;

    @Schema(description = "创建时间")
    private LocalDateTime createTime;

    @Schema(description = "创建人")
    private String createBy;
}
