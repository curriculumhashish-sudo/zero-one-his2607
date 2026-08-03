package com.zeroone.star.project.dto.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.math.BigDecimal;

/**
 * 新增器材DTO
 */
@Data
@Schema(description = "新增器材DTO")
public class EquipmentAddDTO {

    /** 器材业务编号 */
    @Schema(description = "耗材业务编号", example = "DD20260001")
    private String busNo;

    /** 器材名称 */
    @NotBlank(message = "器材名称不能为空")
    @Schema(description = "器材名称", example = "一次性无菌注射器", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /** 拼音检索码（可选；不传则由领域按名称生成） */
    @Schema(description = "拼音检索码（可选；不传则由领域按名称生成）", example = "YCXWMDZSQ")
    private String pyStr;

    /** 五笔检索码（可选；不传则由领域按名称生成） */
    @Schema(description = "五笔检索码（可选；不传则由领域按名称生成）", example = "GFTFWURMA")
    private String wbStr;

    /** 商品名称 */
    @Schema(description = "商品名称", example = "康乐一次性注射器")
    private String merchandiseName;

    /** 器材分类字典编码 */
    @NotBlank(message = "器材分类字典编码不能为空")
    @Schema(description = "器材分类字典编码", example = "11", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    /** 器材类型编码 */
    @Schema(description = "器材类型编码", example = "2")
    private String typeCode;

    /** 采购单位编码 */
    @NotBlank(message = "采购单位编码不能为空")
    @Schema(description = "采购单位编码", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
    private String purchaseUnitCode;

    /** 包装规格（与药品 totalVolume 对齐） */
    @NotBlank(message = "包装规格不能为空")
    @Schema(description = "包装规格（与药品 totalVolume 对齐）", example = "10支/盒", requiredMode = Schema.RequiredMode.REQUIRED)
    private String totalVolume;

    /** 拆零单位编码 */
    @NotBlank(message = "拆零单位编码不能为空")
    @Schema(description = "拆零单位编码", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
    private String splitUnitCode;

    /** 采购单位历史别名（purchaseUnitCode 为空时回落）；新接口请传 purchaseUnitCode */
    @NotBlank(message = "采购单位历史别名不能为空")
    @Schema(description = "采购单位历史别名（purchaseUnitCode 为空时回落）；新接口请传 purchaseUnitCode", example = "2")
    private String unitCode;

    /** 拆零单位历史别名（splitUnitCode 为空时回落）；新接口请传 splitUnitCode */
    @NotBlank(message = "拆零单位历史别名不能为空")
    @Schema(description = "拆零单位历史别名（splitUnitCode 为空时回落）；新接口请传 splitUnitCode", example = "2")
    private String minUnitCode;

    /** 默认门诊/就诊单位编码 */
    @Schema(description = "默认门诊/就诊单位编码", example = "2")
    private String defEncounterUnitCode;

    /** 默认住院单位编码 */
    @Schema(description = "默认住院单位编码", example = "2")
    private String defInhospitalUnitCode;

    /** 包装单位编码（界面命名；持久化列 package_unit_code） */
    @NotBlank(message = "包装单位编码不能为空")
    @Schema(description = "包装单位编码（界面命名；持久化列 package_unit_code）", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
    private String packageUnitCode;

    /** 库存单位编码（持久化列 stock_unit_code） */
    @NotBlank(message = "库存单位编码不能为空")
    @Schema(description = "库存单位编码（持久化列 stock_unit_code）", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
    private String stockUnitCode;

    /** 剂量单位编码（给药/化验口径） */
    @NotBlank(message = "剂量单位编码不能为空")
    @Schema(description = "剂量单位编码（给药/化验口径）", example = "2")
    private String doseUnitCode;

    /** 执行机构主键 */
    @Schema(description = "执行机构主键", example = "1001")
    private Long orgId;

    /** 默认存放位置主键 */
    @Schema(description = "默认存放位置主键", example = "2001")
    private Long locationId;

    /** 设备型号 */
    @NotBlank(message = "设备型号不能为空")
    @Schema(description = "设备型号", example = "KL-5ML")
    private String modelNumber;

    /** 是否高值耗材 */
    @Schema(description = "是否高值耗材", example = "false")
    private Boolean hvcmFlag;

    /** 批准文号 */
    @Schema(description = "批准文号", example = "国械注准20203140001")
    private String approvalNumber;

    /** 是否医保耗材 */
    @Schema(description = "是否医保耗材", example = "true")
    private Boolean ybFlag;

    /** 医保目录编码 */
    @Schema(description = "医保目录编码", example = "C0101010010000101199")
    private String ybNo;

    /** 医保机构编码 */
    @Schema(description = "医保机构编码", example = "P1101010010000101199")
    private String ybOrgNo;

    /** 医保对码是否完成 */
    @Schema(description = "医保对码是否完成", example = "true")
    private Boolean ybMatchFlag;

    /** 医保收费等级 */
    @Schema(description = "医保收费等级", example = "1")
    private Integer chrgitmLv;

    /** 成分或材质说明 */
    @Schema(description = "成分或材质说明", example = "聚丙烯、不锈钢针")
    private String substanceText;

    /** 生产厂家主键 */
    @Schema(description = "生产厂家主键", example = "3001")
    private Long manufacturerId;

    /** 生产厂家名称 */
    @Schema(description = "生产厂家名称", example = "康乐医疗器械有限公司")
    private String manufacturerText;

    /** 供应商主键 */
    @Schema(description = "供应商主键", example = "4001")
    private Long supplyId;

    /** 是否过敏原相关耗材 */
    @Schema(description = "是否过敏原相关耗材", example = "false")
    private Boolean allergenFlag;

    /** 财务分类编码（收费侧） */
    @NotBlank(message = "财务分类编码不能为空")
    @Schema(description = "财务分类编码（收费侧）", example = "FIN001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemTypeCode;

    /** 医保类别 */
    @Schema(description = "医保类别", example = "A")
    private String ybType;

    /** 管辖范围或适用范围说明 */
    @Schema(description = "管辖范围或适用范围说明", example = "适用于医疗机构皮下、肌肉注射")
    private String jurisdiction;

    /** 采购价（元） */
    @NotNull(message = "采购价不能为空")
    @DecimalMin(value = "0.00", message = "采购价不能小于0")
    @Schema(description = "采购价（元）", example = "15.50", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal purchasePrice;

    /** 零售价（元） */
    @NotNull(message = "零售价不能为空")
    @DecimalMin(value = "0.00", message = "零售价不能小于0")
    @Schema(description = "零售价（元）", example = "25.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal retailPrice;

    /** 最高零售价/限价（元） */
    @Schema(description = "最高零售价/限价（元）", example = "30.00")
    private BigDecimal maximumRetailPrice;

    /** 最小警戒数量 */
    @Schema(description = "最小警戒数量", example = "10")
    private BigDecimal itemMinQuantity;

    /** 最大警戒数量 */
    @Schema(description = "最大警戒数量", example = "1000")
    private BigDecimal itemMaxQuantity;

    /** 补充描述 */
    @Schema(description = "补充描述", example = "5ml规格，无菌包装")
    private String description;

    /** 五单位换算比：拆零单位为 1，其余为相对倍数 */
    @Schema(description = "五单位换算比：拆零单位为 1，其余为相对倍数", example = "1.0")
    private BigDecimal doseUnitRatio;

    /** 采购单位相对拆零单位换算比 */
    @NotNull(message = "采购单位换算比不能为空")
    @DecimalMin(value = "0.00", message = "采购单位换算比不能小于0")
    @Schema(description = "采购单位相对拆零单位换算比", example = "1.0", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal purchaseUnitRatio;

    /** 库存单位相对拆零单位换算比 */
    @NotNull(message = "库存单位换算比不能为空")
    @DecimalMin(value = "0.00", message = "库存单位换算比不能小于0")
    @Schema(description = "库存单位相对拆零单位换算比", example = "1.0", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal stockUnitRatio;

    /** 销售单位相对拆零单位换算比 */
    @NotNull(message = "销售单位换算比不能为空")
    @DecimalMin(value = "0.00", message = "销售单位换算比不能小于0")
    @Schema(description = "销售单位相对拆零单位换算比", example = "1.0", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal packageUnitRatio;

}