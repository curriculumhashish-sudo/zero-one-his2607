package com.zeroone.star.project.query.j7.pharmacy.approval;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
public class StockTakeQuery extends PageQuery {

    /**
     * 供应请求聚合根（同表多行：同一 { bus_no} 聚合多行明细，共享审批状态）
     */
    @Schema(description = "盘点单明细编号")
    private String busNo;
}
