package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：器材管理实体
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@TableName("adm_device_definition")
@Schema(description = "器材定义")
public class AdmDeviceDefinition {

    /**
     * 主键标识
     */
    //TODO 主键是自增长还是基于雪花算法之类的还待商榷
    @TableId(type = IdType.AUTO)
    @Schema(description = "主键标识")
    private Long id;

    /**
     * 器材名称
     */
    @TableField("name")
    @Schema(description = "器材名称")
    private String name;

    /**
     * 商品名称
     */
    @TableField("merchandise_name")
    @Schema(description = "商品名称")
    private String merchandiseName;

    /**
     * 商品名拼音检索码
     */
    @TableField("merchandise_py_str")
    @Schema(description = "商品名拼音检索码")
    private String merchandisePyStr;

    /**
     * 商品名五笔检索码
     */
    @TableField("merchandise_wb_str")
    @Schema(description = "商品名五笔检索码")
    private String merchandiseWbStr;

    /**
     * 器材业务编码（院内唯一）
     */
    @TableField("bus_no")
    @Schema(description = "器材业务编码（院内唯一）")
    private String busNo;

    /**
     * 拼音码（用于检索）
     */
    @TableField("py_str")
    @Schema(description = "拼音码（用于检索）")
    private String pyStr;

    /**
     * 五笔码（用于检索）
     */
    @TableField("wb_str")
    @Schema(description = "五笔码（用于检索）")
    private String wbStr;

    /**
     * 器材分类编码
     */
    @TableField("category_code")
    @Schema(description = "器材分类编码")
    private String categoryCode;

    /**
     * 器材种类编码
     */
    @TableField("type_code")
    @Schema(description = "器材种类编码")
    private String typeCode;

    /**
     * 采购单位（5类单位之一）
     */
    @TableField("purchase_unit_code")
    @Schema(description = "采购单位（5类单位之一）")
    private String purchaseUnitCode;

    /**
     * 包装规格描述（与药品 total_volume 语义对齐）
     */
    @TableField("total_volume")
    @Schema(description = "包装规格描述（与药品 total_volume 语义对齐）")
    private String totalVolume;

    /**
     * 最小库存警戒量
     */
    @TableField("item_min_quantity")
    @Schema(description = "最小库存警戒量")
    private BigDecimal itemMinQuantity;

    /**
     * 最大库存警戒量
     */
    @TableField("item_max_quantity")
    @Schema(description = "最大库存警戒量")
    private BigDecimal itemMaxQuantity;

    /**
     * 拆零单位（5类单位之一）
     */
    @TableField("split_unit_code")
    @Schema(description = "拆零单位（5类单位之一）")
    private String splitUnitCode;

    /**
     * 所属科室标识
     */
    @TableField("org_id")
    @Schema(description = "所属科室标识")
    private Long orgId;

    /**
     * 所在位置标识
     */
    @TableField("location_id")
    @Schema(description = "所在位置标识")
    private Long locationId;

    /**
     * 默认门诊/就诊单位编码
     */
    @TableField("def_encounter_unit_code")
    @Schema(description = "默认门诊/就诊单位编码")
    private String defEncounterUnitCode;

    /**
     * 默认住院单位编码
     */
    @TableField("def_inhospital_unit_code")
    @Schema(description = "默认住院单位编码")
    private String defInhospitalUnitCode;

    /**
     * 产品型号
     */
    @TableField("model_number")
    @Schema(description = "产品型号")
    private String modelNumber;

    /**
     * 高值器材标志
     */
    @TableField("hvcm_flag")
    @Schema(description = "高值器材标志")
    private Boolean hvcmFlag;

    /**
     * 销售单位（5类单位之一）
     */
    @TableField("package_unit_code")
    @Schema(description = "销售单位（5类单位之一）")
    private String packageUnitCode;

    /**
     * 库存单位（5类单位之一）
     */
    @TableField("stock_unit_code")
    @Schema(description = "库存单位（5类单位之一）")
    private String stockUnitCode;

    /**
     * 剂量单位（5类单位之一）
     */
    @TableField("dose_unit_code")
    @Schema(description = "剂量单位（5类单位之一）")
    private String doseUnitCode;

    /**
     * 剂量单位→拆零单位换算倍数（拆零单位为1）
     */
    @TableField("dose_unit_ratio")
    @Schema(description = "剂量单位→拆零单位换算倍数（拆零单位为1）")
    private BigDecimal doseUnitRatio;

    /**
     * 采购单位→拆零单位换算倍数（拆零单位为1）
     */
    @TableField("purchase_unit_ratio")
    @Schema(description = "采购单位→拆零单位换算倍数（拆零单位为1）")
    private BigDecimal purchaseUnitRatio;

    /**
     * 库存单位→拆零单位换算倍数（拆零单位为1）
     */
    @TableField("stock_unit_ratio")
    @Schema(description = "库存单位→拆零单位换算倍数（拆零单位为1）")
    private BigDecimal stockUnitRatio;

    /**
     * 销售单位→拆零单位换算倍数（拆零单位为1）
     */
    @TableField("package_unit_ratio")
    @Schema(description = "销售单位→拆零单位换算倍数（拆零单位为1）")
    private BigDecimal packageUnitRatio;

    /**
     * 批准文号
     */
    @TableField("approval_number")
    @Schema(description = "批准文号")
    private String approvalNumber;

    /**
     * 生产厂家标识
     */
    @TableField("manufacturer_id")
    @Schema(description = "生产厂家标识")
    private Long manufacturerId;

    /**
     * 生产厂家名称
     */
    @TableField("manufacturer_text")
    @Schema(description = "生产厂家名称")
    private String manufacturerText;

    /**
     * 供应商标识
     */
    @TableField("supply_id")
    @Schema(description = "供应商标识")
    private Long supplyId;

    /**
     * 器材说明
     */
    @TableField("description")
    @Schema(description = "器材说明")
    private String description;

    /**
     * 适用范围描述
     */
    @TableField("jurisdiction")
    @Schema(description = "适用范围描述")
    private String jurisdiction;

    /**
     * 器材版本
     */
    @TableField("device_version")
    @Schema(description = "器材版本")
    private String version;

    /**
     * 主要成分描述
     */
    @TableField("substance_text")
    @Schema(description = "主要成分描述")
    private String substanceText;

    /**
     * 过敏标记
     */
    @TableField("allergen_flag")
    @Schema(description = "过敏标记")
    private Boolean allergenFlag;

    /**
     * 处方标志
     */
    @TableField("rx_flag")
    @Schema(description = "处方标志")
    private Boolean rxFlag;

    /**
     * 医保标记
     */
    @TableField("yb_flag")
    @Schema(description = "医保标记")
    private Boolean ybFlag;

    /**
     * 医保编码
     */
    @TableField("yb_no")
    @Schema(description = "医保编码")
    private String ybNo;

    /**
     * 医药机构目录编码
     */
    @TableField("yb_org_no")
    @Schema(description = "医药机构目录编码")
    private String ybOrgNo;

    /**
     * 医保对码标记
     */
    @TableField("yb_match_flag")
    @Schema(description = "医保对码标记")
    private Boolean ybMatchFlag;

    /**
     * 医保等级
     */
    @TableField("chrgitm_lv")
    @Schema(description = "医保等级")
    private Integer chrgitmLv;

    /**
     * 发布状态（存储为整数码）
     */
    @TableField("status_enum")
    @Schema(description = "发布状态（存储为整数码）")
    private String statusEnum;

    /**
     * 是否启用标记。
     *
     * <p>DOC-CONFLICT：DM/SDD 未定义 activeFlag 字段；存量表与代码存在该列，当前保留以兼容既有查询/前端展示口径。</p>
     */
    @TableField("active_flag")
    @Schema(description = "是否启用标记（DOC-CONFLICT：DM/SDD 未定义，存量兼容保留）")
    private Boolean activeFlag;

    /**
     * 医保对码标识。
     *
     * <p>DOC-CONFLICT：DM/SDD 定义的医保回写字段为 ybFlag/ybNo/ybOrgNo/ybMatchFlag/chrgitmLv；
     * 存量实现使用 ybMappingCode。当前保留该列以兼容存量调用方，但回写方法优先按 SDD 字段全集落地。</p>
     */
    @TableField("yb_mapping_code")
    @Schema(description = "医保对码标识（DOC-CONFLICT：存量兼容保留）")
    private String ybMappingCode;
}