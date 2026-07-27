package com.zeroone.star.project.dto.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：住院退药确认明细项
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author Jacobzp
 * @version 1.0.0
 */
@Data
@Schema(description = "住院退药确认明细项")
public class InpatientRefundConfirmItemDTO {

    @NotBlank(message = "发药记录ID不能为空")
    @Schema(description = "发药记录ID", example = "329906419476205568", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dispenseId;
}
