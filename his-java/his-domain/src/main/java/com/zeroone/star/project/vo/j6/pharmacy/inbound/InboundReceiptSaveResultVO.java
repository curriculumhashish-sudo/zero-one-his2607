package com.zeroone.star.project.vo.j6.pharmacy.inbound;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 进货单写操作结果展示对象。
 */
@Data
@Schema(name = "InboundReceiptSaveResultVO", description = "药库进货单新增/保存/提交结果")
public class InboundReceiptSaveResultVO {
    @Schema(description = "供应请求主键，落库后返回", example = "510000000000000001")
    private String id;

    @Schema(description = "业务单号", example = "JHD202607190001")
    private String busNo;

    @Schema(description = "供应状态编码", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "乐观锁版本号", example = "0")
    private Integer version;

    @Schema(description = "操作结果说明", example = "保存成功")
    private String actionResult;

    @Schema(description = "申请时间，保存并提交审批时返回", example = "2026-07-19T10:35:00")
    private LocalDateTime applyTime;
}
