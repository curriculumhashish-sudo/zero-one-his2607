package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.math.BigDecimal;

/**
 * <p>
 * 描述：药品多单位包装VO
 * </p>
 * @author 01星球
 */
@Data
@Schema(description = "药品多单位包装信息")
public class UnitBandVO {
    @Schema(description = "包装类型")
    private String bandKind;

    @Schema(description = "单位编码")
    private String unitCode;

    @Schema(description = "单位名称", example = "g")
    private String unitName;

    @Schema(description = "换算比例")
    private BigDecimal ratioToSplit;

    @Schema(description = "单位价格")
    private BigDecimal unitPrice;
}
