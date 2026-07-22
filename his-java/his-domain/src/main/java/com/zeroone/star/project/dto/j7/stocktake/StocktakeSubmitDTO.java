package com.zeroone.star.project.dto.j7.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：盘点单提交审批请求
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单提交审批请求")
public class StocktakeSubmitDTO {

    @NotBlank(message = "盘点单号不能为空")
    @Schema(description = "盘点单业务单号", example = "PD202607230001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "提交审批备注", example = "盘点完成，提交审批")
    private String approvalRemark;
}
