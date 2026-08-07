package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;
import java.util.List;

/**
 * 药房管理-药房入库单-提交审批请求对象
 */
@Data
@Schema(description = "提交审批请求对象")
public class PurchaseInboundSubmitDTO {

    @NotBlank(message = "入库单ID不能为空")
    @Size(max = 64, message = "入库单ID长度不能超过64个字符")
    @Schema(description = "入库单ID (若为纯状态流转接口则必填)", example = "1")
    private String id;

    @Valid
    @Schema(description = "入库单明细列表 (若为保存并提交接口则必填)")
    private List<PurchaseInboundItemDTO> items;

    @Size(max = 255, message = "备注信息长度不能超过255个字符")
    @Schema(description = "备注信息", example = "紧急补货")
    private String remark;
}