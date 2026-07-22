package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;

@Schema(description = "审批DTO")
public class ApproveDTO {

    /**
     * 供应请求聚合根（同表多行：同一 { bus_no} 聚合多行明细，共享审批状态）
     */
    @Schema(description = "盘点单明细编号")
    @NotNull(message = "盘点单明细编号不能为空")
    private String busNo;

    /**
     * approver标识
     */
    @Schema(description = "审批人标识")
    @NotNull(message = "审批人标识不能为空")
    private String approverId;

    @Schema(description = "备注(驳回需要备注)")
    private String remark;
}
