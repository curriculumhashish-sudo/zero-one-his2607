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
    private String id;

    @Schema(description = "业务编号")
    private String busNo;

    @Schema(description = "器材名称")
    private String name;

    @Schema(description = "拼音码")
    private String pyStr;

    @Schema(description = "五笔码")
    private String wbStr;

    @Schema(description = "商品名称")
    private String merchandiseName;

    @Schema(description = "商品拼音码")
    private String merchandisePyStr;

    @Schema(description = "商品五笔码")
    private String merchandiseWbStr;

    @Schema(description = "器材分类编码")
    private String categoryCode;

    @Schema(description = "器材分类名称")
    private String categoryName;

    @Schema(description = "器材种类编码")
    private String typeCode;

    @Schema(description = "器材种类名称")
    private String typeName;

    @Schema(description = "财务/项目类型编码")
    private String itemTypeCode;

    @Schema(description = "财物/项目类型名称")
    private String itemTypeCode_dictText;

    @Schema(description = "规格型号")
    private String modelNumber;

    @Schema(description = "规格")
    private String specification;

    @Schema(description = "包装规格")
    private String totalVolume;

    @Schema(description = "采购单位编码")
    private String purchaseUnitCode;

    @Schema(description = "采购单位名称")
    private String purchaseUnitName;

    @Schema(description = "单位编码")
    private String unitCode;

    @Schema(description = "单位名称")
    private String unitCode_dictText;

    @Schema(description = "剂量单位编码")
    private String doseUnitCode;

    @Schema(description = "剂量单位名称")
    private String doseUnitCode_dictText;

    @Schema(description = "拆零单位编码")
    private String splitUnitCode;

    @Schema(description = "拆零单位名称")
    private String splitUnitName;

    @Schema(description = "库存单位编码")
    private String stockUnitCode;

    @Schema(description = "库存单位名称")
    private String stockUnitName;

    @Schema(description = "销售单位编码")
    private String packageUnitCode;

    @Schema(description = "销售单位名称")
    private String packageUnitName;

    @Schema(description = "剂量单位比例")
    private BigDecimal doseUnitRatio;

    @Schema(description = "采购单位比例")
    private BigDecimal purchaseUnitRatio;

    @Schema(description = "库存单位比例")
    private BigDecimal stockUnitRatio;

    @Schema(description = "包装单位比例")
    private BigDecimal packageUnitRatio;

    /**
     * 对应PURCHASE
     */
    @Schema(description = "采购价（元）")
    private BigDecimal purchasePrice;

    /**
     * 对应SALE
     */
    @Schema(description = "零售价（元）")
    private BigDecimal retailPrice;

    /**
     * 对应LIMIT
     */
    @Schema(description = "最高零售价（元）")
    private BigDecimal maximumRetailPrice;

    @Schema(description = "最小数量")
    private BigDecimal itemMinQuantity;

    @Schema(description = "最大数量")
    private BigDecimal itemMaxQuantity;

    @Schema(description = "生产厂家ID")
    private String manufacturerId;

    @Schema(description = "生产厂家")
    private String manufacturerText;

    @Schema(description = "供应商ID")
    private String supplyId;

    @Schema(description = "供应商名称")
    private String supplyId_dictText;

    @Schema(description = "批准文号")
    private String approvalNumber;

    @Schema(description = "是否过敏原")
    private Boolean allergenFlag;

    @Schema(description = "是否医保")
    private Boolean ybFlag;

    @Schema(description = "医保编码")
    private String ybNo;

    @Schema(description = "医药机构目录编码")
    private String ybOrgNo;

    @Schema(description = "是否医保匹配")
    private Boolean ybMatchFlag;

    @Schema(description = "医保类型编码")
    private String ybType;

    @Schema(description = "医保类型名称")
    private String ybType_dictText;

    @Schema(description = "收费项目/医保 等级")
    private String chrgitmLv;

    @Schema(description = "是否高值耗材")
    private Boolean hvcmFlag;

    @Schema(description = "可编辑性编码")
    private String editabilityCode;

    @Schema(description = "生命状态（ACTIVE/INACTIVE）")
    private String statusEnum;

    @Schema(description = "活跃标志")
    private Boolean activeFlag;

    @Schema(description = "描述")
    private String description;

    /**
     * 价格类型编码,只负责后端数据传递,前端不做接收
     */
    private String priceTypeCode;
}