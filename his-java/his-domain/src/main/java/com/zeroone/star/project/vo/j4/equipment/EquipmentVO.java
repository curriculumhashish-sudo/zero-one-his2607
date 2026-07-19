package com.zeroone.star.project.vo.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * 器材目录 VO
 */
@Data
@Schema(description = "器材VO")
public class EquipmentVO {

    @Schema(description = "主键")
    private Long id;

    @Schema(description = "业务编号")
    private String busNo;

    @Schema(description = "器材名称")
    private String name;

    @Schema(description = "器材分类编码")
    private String categoryCode;

    @Schema(description = "器材分类名称")
    private String categoryName;

    @Schema(description = "器材种类编码")
    private String typeCode;

    @Schema(description = "器材种类名称")
    private String typeName;

    @Schema(description = "规格型号")
    private String modelNumber;

    @Schema(description = "采购单位名称")
    private String purchaseUnitName;

    @Schema(description = "包装规格")
    private String totalVolume;

    @Schema(description = "拆零单位名称")
    private String splitUnitName;

    @Schema(description = "库存单位名称")
    private String stockUnitName;

    @Schema(description = "销售单位名称")
    private String packageUnitName;

    @Schema(description = "采购价（元）")
    private BigDecimal purchasePrice;

    @Schema(description = "零售价（元）")
    private BigDecimal retailPrice;

    @Schema(description = "生产厂家")
    private String manufacturerText;

    @Schema(description = "是否医保")
    private Boolean ybFlag;

    @Schema(description = "医保编码")
    private String ybNo;

    @Schema(description = "是否高值耗材")
    private Boolean hvcmFlag;

    @Schema(description = "生命状态（ACTIVE/INACTIVE）")
    private String statusEnum;

    @Schema(description = "生命状态中文名")
    private String statusName;

    @Schema(description = "描述")
    private String description;
}