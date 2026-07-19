package com.zeroone.star.project.dto.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * 新增耗材目录请求（字段对齐前端建档表单）。
 */
@Data
@Schema(description = "新增耗材目录请求")
public class EquipmentAddDTO {

    /** 耗材业务编号 */
    @Schema(description = "耗材业务编号")
    private String busNo;

    /** 耗材名称 */
    @Schema(description = "耗材名称")
    private String name;

    /** 拼音检索码（可选；不传则由领域按名称生成） */
    @Schema(description = "拼音检索码（可选；不传则由领域按名称生成）")
    private String pyStr;

    /** 五笔检索码（可选；不传则由领域按名称生成） */
    @Schema(description = "五笔检索码（可选；不传则由领域按名称生成）")
    private String wbStr;

    /** 商品名称 */
    @Schema(description = "商品名称")
    private String merchandiseName;

    /** 耗材分类字典编码 */
    @Schema(description = "耗材分类字典编码")
    private String categoryCode;

    /** 耗材类型编码 */
    @Schema(description = "耗材类型编码")
    private String typeCode;

    /** 采购单位编码（持久化列 purchase_unit_code；与药品目录字段名对齐） */
    @Schema(description = "采购单位编码（持久化列 purchase_unit_code；与药品目录字段名对齐）")
    private String purchaseUnitCode;

    /** 包装规格（与药品 totalVolume 对齐） */
    @Schema(description = "包装规格（与药品 totalVolume 对齐）")
    private String totalVolume;

    /** 拆零单位编码（持久化列 split_unit_code；与药品目录字段名对齐） */
    @Schema(description = "拆零单位编码（持久化列 split_unit_code；与药品目录字段名对齐）")
    private String splitUnitCode;

    /** 采购单位历史别名（purchaseUnitCode 为空时回落）；新接口请传 purchaseUnitCode */
    @Schema(description = "采购单位历史别名（purchaseUnitCode 为空时回落）；新接口请传 purchaseUnitCode")
    private String unitCode;

    /** 拆零单位历史别名（splitUnitCode 为空时回落）；新接口请传 splitUnitCode */
    @Schema(description = "拆零单位历史别名（splitUnitCode 为空时回落）；新接口请传 splitUnitCode")
    private String minUnitCode;

    /** 默认门诊/就诊单位编码 */
    @Schema(description = "默认门诊/就诊单位编码")
    private String defEncounterUnitCode;

    /** 默认住院单位编码 */
    @Schema(description = "默认住院单位编码")
    private String defInhospitalUnitCode;

    /** 包装单位编码（界面命名；持久化列 package_unit_code） */
    @Schema(description = "包装单位编码（界面命名；持久化列 package_unit_code）")
    private String packageUnitCode;

    /** 库存单位编码（持久化列 stock_unit_code） */
    @Schema(description = "库存单位编码（持久化列 stock_unit_code）")
    private String stockUnitCode;

    /** 剂量单位编码（给药/化验口径） */
    @Schema(description = "剂量单位编码（给药/化验口径）")
    private String doseUnitCode;

    /** 执行机构主键 */
    @Schema(description = "执行机构主键")
    private Long orgId;

    /** 默认存放位置主键 */
    @Schema(description = "默认存放位置主键")
    private Long locationId;

    /** 设备型号 */
    @Schema(description = "设备型号")
    private String modelNumber;

    /** 是否高值耗材 */
    @Schema(description = "是否高值耗材")
    private Boolean hvcmFlag;

    /** 批准文号 */
    @Schema(description = "批准文号")
    private String approvalNumber;

    /** 是否医保耗材 */
    @Schema(description = "是否医保耗材")
    private Boolean ybFlag;

    /** 医保目录编码 */
    @Schema(description = "医保目录编码")
    private String ybNo;

    /** 医保机构编码 */
    @Schema(description = "医保机构编码")
    private String ybOrgNo;

    /** 医保对码是否完成 */
    @Schema(description = "医保对码是否完成")
    private Boolean ybMatchFlag;

    /** 医保收费等级 */
    @Schema(description = "医保收费等级")
    private Integer chrgitmLv;

    /** 成分或材质说明 */
    @Schema(description = "成分或材质说明")
    private String substanceText;

    /** 生产厂家主键 */
    @Schema(description = "生产厂家主键")
    private Long manufacturerId;

    /** 生产厂家名称 */
    @Schema(description = "生产厂家名称")
    private String manufacturerText;

    /** 供应商主键 */
    @Schema(description = "供应商主键")
    private Long supplyId;

    /** 是否过敏原相关耗材 */
    @Schema(description = "是否过敏原相关耗材")
    private Boolean allergenFlag;

    /** 财务分类编码（收费侧） */
    @Schema(description = "财务分类编码（收费侧）")
    private String itemTypeCode;

    /** 医保类别 */
    @Schema(description = "医保类别")
    private String ybType;

    /** 管辖范围或适用范围说明 */
    @Schema(description = "管辖范围或适用范围说明")
    private String jurisdiction;

    /** 采购价（元） */
    @Schema(description = "采购价（元）")
    private BigDecimal purchasePrice;

    /** 零售价（元） */
    @Schema(description = "零售价（元）")
    private BigDecimal retailPrice;

    /** 最高零售价/限价（元） */
    @Schema(description = "最高零售价/限价（元）")
    private BigDecimal maximumRetailPrice;

    /** 最小警戒数量 */
    @Schema(description = "最小警戒数量")
    private BigDecimal itemMinQuantity;

    /** 最大警戒数量 */
    @Schema(description = "最大警戒数量")
    private BigDecimal itemMaxQuantity;

    /** 补充描述 */
    @Schema(description = "补充描述")
    private String description;

    /** 五单位换算比：拆零单位为 1，其余为相对倍数（持久化语义见领域层） */
    @Schema(description = "五单位换算比：拆零单位为 1，其余为相对倍数（持久化语义见领域层）")
    private BigDecimal doseUnitRatio;

    /** 采购单位相对拆零单位换算比 */
    @Schema(description = "采购单位相对拆零单位换算比")
    private BigDecimal purchaseUnitRatio;

    /** 库存单位相对拆零单位换算比 */
    @Schema(description = "库存单位相对拆零单位换算比")
    private BigDecimal stockUnitRatio;

    /** 销售单位相对拆零单位换算比 */
    @Schema(description = "销售单位相对拆零单位换算比")
    private BigDecimal packageUnitRatio;

}