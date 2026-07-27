package com.zeroone.star.project.vo.j7.dispense;

import com.zeroone.star.project.vo.OptionItemVO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：住院退药页面初始化数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author Jacobzp
 * @version 1.0.0
 */
@Data
@Schema(description = "住院退药页面初始化数据")
public class InpatientRefundInitVO {

    @Schema(description = "病区选项")
    private List<OptionItemVO> wardOptions;

    @Schema(description = "退药状态选项")
    private List<OptionItemVO> refundStatusOptions;
}
