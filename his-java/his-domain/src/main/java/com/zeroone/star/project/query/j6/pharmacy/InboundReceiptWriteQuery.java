package com.zeroone.star.project.query.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 药库进货单写操作结果查询对象。
 */
@Data
@Schema(name = "InboundReceiptWriteQuery", description = "药库进货单新增/保存/提交后的单据定位条件")
public class InboundReceiptWriteQuery {
    @NotBlank(message = "业务单号不能为空")
    @Schema(description = "业务单号", example = "JHD202607190001")
    private String busNo;

    @Schema(description = "供应状态编码", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "乐观锁版本号", example = "1")
    private Integer version;
}
