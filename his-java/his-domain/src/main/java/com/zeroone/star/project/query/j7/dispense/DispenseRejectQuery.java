package com.zeroone.star.project.query.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import jakarta.validation.constraints.NotBlank;

/**
 * <p>
 * 描述：住院发药驳回查询对象
 * </p>
 * <p>
 * 继承 DispensingSummaryQuery，新增驳回原因字段，
 * 用于住院发药驳回操作。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Data
@Schema(description = "住院发药驳回请求")
public class DispenseRejectQuery {

    @NotBlank(message = "汇总单号不能为空")
    @Schema(description = "汇总单号（测试库暂无住院发药汇总单数据）", example = "HZ20240719001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String summaryNo;

    @NotBlank(message = "驳回原因不能为空")
    @Schema(description = "驳回原因", requiredMode = Schema.RequiredMode.REQUIRED)
    private String rejectReason;
}