package com.zeroone.star.project.vo.j4.medicine;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * <p>
 * 描述：选择药品列表显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "选择药品列表显示对象")
public class MedicineSelectVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "药品ID")
    private String id;

    @Schema(description = "项目编码")
    private String itemCode;

    @Schema(description = "项目名称")
    private String itemName;

    @Schema(description = "类别名称")
    private String categoryName;

    @Schema(description = "规格")
    private String specification;

    @Schema(description = "库存数量")
    private BigDecimal stockQuantity;

    @Schema(description = "库存单位编码")
    private String stockUnitCode;

    @Schema(description = "单价")
    private BigDecimal unitPrice;
}
