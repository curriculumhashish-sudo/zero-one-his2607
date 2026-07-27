package com.zeroone.star.project.dto.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：住院退药确认请求
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author Jacobzp
 * @version 1.0.0
 */
@Data
@Schema(description = "住院退药确认请求")
public class InpatientRefundConfirmDTO {

    @Valid
    @NotEmpty(message = "退药明细不能为空")
    @Schema(description = "退药明细列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<InpatientRefundConfirmItemDTO> items;

    @Schema(description = "退药原因", example = "患者停药")
    private String reason;
}
