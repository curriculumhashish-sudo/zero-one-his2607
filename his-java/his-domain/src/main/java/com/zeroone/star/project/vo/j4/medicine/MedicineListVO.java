package com.zeroone.star.project.vo.j4.medicine;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * <p>
 * 描述：药品列表显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "药品列表显示对象")
public class MedicineListVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "药品ID", example = "323999010000000015")
    private String id;

    @Schema(description = "药品编码", example = "MD0034")
    private String busNo;

    @Schema(description = "通用名称", example = "茯苓")
    private String name;

    @Schema(description = "规格", example = "饮片")
    private String totalVolume;

    @Schema(description = "是否启用", example = "true")
    private Boolean activeFlag;

    @Schema(description = "药品分类编码", example = "4")
    private String categoryCode;

    @Schema(description = "生产厂家", example = "安国市中药材市场")
    private String manufacturerText;

    @Schema(description = "零售价", example = "0.06")
    private BigDecimal retailPrice;

    @Schema(description = "包装单位编码", example = "10")
    private String packageUnitCode;
}