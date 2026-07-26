package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：住院退药确认结果
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author Jacobzp
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "住院退药确认结果")
public class InpatientRefundConfirmVO {

    @Schema(description = "是否成功", example = "true")
    private Boolean success;

    @Schema(description = "提示信息", example = "确认退药成功")
    private String message;
}
