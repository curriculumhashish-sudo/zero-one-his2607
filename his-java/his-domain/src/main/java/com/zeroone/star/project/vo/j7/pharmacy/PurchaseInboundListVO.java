package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 药房管理-历史入库单-列表响应对象
 * 对应图片：历史入库单列表项
 */
@Data
@Schema(description = "历史入库单列表项")
public class PurchaseInboundListVO {

    @Schema(description = "主键ID", example = "329874660147597312")
    private String id;

    @Schema(description = "单据编号", example = "PSI2026-06-300001")
    private String busNo;

    @Schema(description = "单据状态 (中文展示: 同意/待审核)", example = "APPROVED")
    private String statusName;

    @Schema(description = "制单人姓名", example = "张三")
    private String createByName;

    @Schema(description = "制单时间", example = "2026-08-09 08:00")
    private LocalDateTime applyTime;

    @Schema(description = "审批人姓名", example = "李四")
    private String approverName;

    @Schema(description = "审批时间", example = "2026-08-09 09:15")
    private LocalDateTime approveTime;

    @Schema(description = "药源库 (来源库房标识)", example = "西药库")
    private String sourceLocationName;

    @Schema(description = "目的药房 (目的库房标识)", example = "门诊西药房")
    private String purposeLocationName;

    @Schema(description = "总金额", example = "1670.06")
    private BigDecimal totalPrice;
}