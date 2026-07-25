package com.zeroone.star.project.dto.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：盘点单提交审批数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单提交审批")
public class StocktakeSubmitDTO {

    @NotBlank(message = "盘点单ID不能为空")
    @Schema(description = "盘点单ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "提交审批备注", example = "请审批")
    private String remark;
}
