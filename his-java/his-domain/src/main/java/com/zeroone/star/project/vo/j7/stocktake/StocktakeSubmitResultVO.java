package com.zeroone.star.project.vo.j7.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：盘点单保存并提交审批结果
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单保存并提交审批结果")
public class StocktakeSubmitResultVO {

    @Schema(description = "盘点单业务单号", example = "PD202607230001")
    private String busNo;

    @Schema(description = "保存动作：CREATE-新增保存，UPDATE-编辑保存", example = "CREATE")
    private String saveMode;

    @Schema(description = "提交后状态编码", example = "APPROVAL")
    private String statusEnum;

    @Schema(description = "提交后状态名称", example = "审批中")
    private String statusName;

    @Schema(description = "是否提交成功", example = "true")
    private Boolean success;
}
