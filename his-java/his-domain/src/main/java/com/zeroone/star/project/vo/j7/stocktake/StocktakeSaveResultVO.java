package com.zeroone.star.project.vo.j7.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：盘点单保存结果
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单保存结果")
public class StocktakeSaveResultVO {

    @Schema(description = "盘点单业务单号", example = "PD202607230001")
    private String busNo;

    @Schema(description = "保存动作：CREATE-新增保存，UPDATE-编辑保存", example = "CREATE")
    private String saveMode;

    @Schema(description = "保存后状态编码", example = "PENDING_APPROVAL")
    private String statusEnum;

    @Schema(description = "保存后状态名称", example = "待审批")
    private String statusName;

    @Schema(description = "是否保存成功", example = "true")
    private Boolean success;
}
