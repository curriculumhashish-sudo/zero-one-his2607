package com.zeroone.star.project.vo.j7.dispense;

import com.zeroone.star.project.dto.j7.dispense.ReturnDrugRecord;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：住院退药-患者退药明细显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j7
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "住院退药-患者退药明细显示对象")
public class InpatientRefundDetailVO extends ReturnDrugRecord {
}
