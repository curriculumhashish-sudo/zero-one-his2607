package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：驳回出库审批DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j6小组-告别
 * @version 1.0.0
 */
@Data
@Schema(description = "驳回出库审批对象")
public class RejectStockOutDTO {
    /**
     * 业务单号
     */
    @Schema(description = "业务单号", example = "SO202607180001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "业务单号不能为空")
    private String busNo;

    /**
     * 驳回原因
     */
    @Schema(description = "驳回原因", example = "药品库存不足", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "驳回原因不能为空")
    private String rejectReason;
}
