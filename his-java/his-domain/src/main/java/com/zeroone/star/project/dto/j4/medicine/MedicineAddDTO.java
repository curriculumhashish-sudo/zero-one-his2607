package com.zeroone.star.project.dto.j4.medicine;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.math.BigDecimal;

/**
 * 新增药品DTO
 */
@Data
@Schema(description = "新增药品DTO")
public class MedicineAddDTO {

    // 基础信息

    @NotBlank(message = "通用名称不能为空")
    @Size(max = 255, message = "通用名称不能超过255个字符")
    @Schema(description = "通用名称", example = "阿莫西林胶囊",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @NotBlank(message = "商品名称不能为空")
    @Size(max = 255, message = "商品名称不能超过255个字符")
    @Schema(description = "商品名称", example = "阿莫仙",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String merchandiseName;

    @NotBlank(message = "药品分类不能为空")
    @Schema(description = "药品分类编码", example = "2",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    @NotBlank(message = "生产厂家不能为空")
    @Size(max = 200, message = "生产厂家不能超过200个字符")
    @Schema(description = "生产厂家", example = "某某制药有限公司",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String manufacturerText;

    @NotBlank(message = "供应商不能为空")
    @Schema(description = "供应商ID", example = "4100000000000000004",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String supplyId;

    @Schema(description = "批准文号", example = "国药准字H20260001")
    private String approvalNumber;

    @Schema(description = "贯标国家编码")
    private String nationalDrugCode;

    @Schema(description = "医保编码")
    private String ybNo;

    @NotBlank(message = "财务类型不能为空")
    @Schema(description = "财务类型编码", example = "1001",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeCode;

    @Schema(description = "医保等级", example = "1")
    private Integer chrgitmLv;

    @Schema(description = "是否完成医保对码", example = "false")
    private Boolean ybMatchFlag;

    @Schema(description = "医保类别")
    private String ybType;

    // 临床信息

    @NotBlank(message = "药品性质不能为空")
    @Schema(description = "药理作用分类编码",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String pharmacologyCategoryCode;

    @NotBlank(message = "规格不能为空")
    @Schema(description = "规格", example = "0.25g*24粒",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String totalVolume;

    @NotBlank(message = "剂型不能为空")
    @Schema(description = "剂型编码", example = "1",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String doseFormCode;

    @NotBlank(message = "剂量单位不能为空")
    @Schema(description = "剂量单位编码", example = "4",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String doseUnitCode;

    @NotNull(message = "拆零/剂量倍数不能为空")
    @DecimalMin(value = "0", inclusive = false,
            message = "拆零/剂量倍数必须大于0")
    @Schema(description = "拆零/剂量倍数", example = "1",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal doseUnitRatio;

    @NotBlank(message = "门诊拆分属性不能为空")
    @Schema(description = "门诊拆分属性编码",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String partAttributeEnum;

    @NotBlank(message = "住院临时医嘱拆分属性不能为空")
    @Schema(description = "住院临时医嘱拆分属性编码",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String thoPartAttributeEnum;

    @Schema(description = "是否需要皮试", example = "false")
    private Boolean skinTestFlag;

    @Schema(description = "是否为注射药品", example = "false")
    private Boolean injectFlag;

    @Schema(description = "默认用法编码")
    private String methodCode;

    @Schema(description = "默认用药频次编码")
    private String rateCode;

    @Schema(description = "最小用药频次编码")
    private String minRateCode;

    @Schema(description = "最大用药频次编码")
    private String maxRateCode;

    @DecimalMin(value = "0", message = "单次剂量不能小于0")
    @Schema(description = "单次剂量")
    private BigDecimal dose;

    @DecimalMin(value = "0", message = "单次最大剂量不能小于0")
    @Schema(description = "单次最大剂量")
    private BigDecimal maxUnit;

    @DecimalMin(value = "0", message = "用量限定不能小于0")
    @Schema(description = "用量限定")
    private BigDecimal usageLimit;

    @Schema(description = "用法说明")
    private String dosageInstruction;

    @Schema(description = "DDD编码")
    private String dddCode;

    @Schema(description = "DDD单位编码")
    private String dddUnitCode;

    @Schema(description = "限制使用范围")
    private String restrictedScope;

    @Schema(description = "药品说明")
    private String definition;

    // 库存与价格信息

    @NotNull(message = "购入价不能为空")
    @DecimalMin(value = "0", message = "购入价不能小于0")
    @Schema(description = "购入价（包装单位）", example = "15.50",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal purchasePrice;

    @NotNull(message = "零售价不能为空")
    @DecimalMin(value = "0", message = "零售价不能小于0")
    @Schema(description = "零售价（包装单位）", example = "20.00",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal retailPrice;

    @DecimalMin(value = "0", message = "最高零售价不能小于0")
    @Schema(description = "最高零售价（包装单位）", example = "25.00")
    private BigDecimal maximumRetailPrice;

    @NotBlank(message = "采购单位不能为空")
    @Schema(description = "采购单位编码",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String purchaseUnitCode;

    @NotNull(message = "采购/拆零倍数不能为空")
    @DecimalMin(value = "0", inclusive = false,
            message = "采购/拆零倍数必须大于0")
    @Schema(description = "采购/拆零倍数", example = "10",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal purchaseUnitRatio;

    @NotBlank(message = "库存单位不能为空")
    @Schema(description = "库存单位编码",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String stockUnitCode;

    @NotNull(message = "库存/拆零倍数不能为空")
    @DecimalMin(value = "0", inclusive = false,
            message = "库存/拆零倍数必须大于0")
    @Schema(description = "库存/拆零倍数", example = "10",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal stockUnitRatio;

    @NotBlank(message = "包装单位不能为空")
    @Schema(description = "包装单位编码",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String packageUnitCode;

    @NotNull(message = "包装/拆零倍数不能为空")
    @DecimalMin(value = "0", inclusive = false,
            message = "包装/拆零倍数必须大于0")
    @Schema(description = "包装/拆零倍数", example = "10",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal packageUnitRatio;

    @NotBlank(message = "拆零单位不能为空")
    @Schema(description = "拆零单位编码",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String splitUnitCode;

    @Schema(description = "默认采购入库库房ID")
    private String locationId;

    @DecimalMin(value = "0", message = "警戒线最高值不能小于0")
    @Schema(description = "警戒线最高值")
    private BigDecimal itemMaxQuantity;

    @DecimalMin(value = "0", message = "警戒线最低值不能小于0")
    @Schema(description = "警戒线最低值")
    private BigDecimal itemMinQuantity;

    // 检索与业务信息

    @Schema(description = "药品拼音码")
    private String pyStr;

    @Schema(description = "药品五笔码")
    private String wbStr;

    @Schema(description = "商品拼音码")
    private String merchandisePyStr;

    @Schema(description = "商品五笔码")
    private String merchandiseWbStr;

    @Schema(description = "药品69码")
    private String drug69Code;

    @Schema(description = "药品状态", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "领域编码")
    private String domainEnum;

    // 标志信息

    @Schema(description = "基药标识", example = "false")
    private Boolean basicFlag;

    @Schema(description = "儿童用药标识", example = "false")
    private Boolean childrenFlag;

    @Schema(description = "自制药标识", example = "false")
    private Boolean selfFlag;

    @Schema(description = "是否有效", example = "true")
    private Boolean activeFlag;

    @Schema(description = "是否限制使用", example = "false")
    private Boolean restrictedFlag;

    @Schema(description = "抗生素标识", example = "false")
    private Boolean antibioticFlag;

    @Schema(description = "处方药标识", example = "false")
    private Boolean rxFlag;
}