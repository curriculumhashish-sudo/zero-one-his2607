package com.zeroone.star.project.dto.j4.treatment;

import com.zeroone.star.project.vo.j4.treatment.WkfActivityDefinitionChildVO;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.math.BigDecimal;
import java.util.List;

/**
 * 新增诊疗项目DTO
 * <p>字段对齐表 {@code wkf_activity_definition} + 关联收费表 + 子项子表。</p>
 *
 * 【前端对接说明】
 * 1. categoryCode必须传编码值，如"21"代表治疗，"22"代表检验，不是中文
 * 2. busNo业务编号需保证唯一，建议按"AD"+序号规则生成
 * 3. statusEnum不传默认ACTIVE（启用）
 * 4. 价格字段（purchasePrice/retailPrice/maximumRetailPrice）可选，传了则同时写入收费表adm_charge_item_definition
 * 5. typeCode为财务类型编码，ybType为医保类别，两者也写入收费表
 * 6. children子项列表：组合项目/套餐时传入，单项目传空数组或不传
 */
@Data
@Schema(description = "新增诊疗项目DTO")
public class TreatmentAddDTO {

    @NotBlank(message = "诊疗项目名称不能为空")
    @Size(max = 255, message = "诊疗项目名称不能超过255个字符")
    @Schema(description = "诊疗项目名称", example = "血常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Size(max = 255, message = "拼音检索码不能超过255个字符")
    @Schema(description = "拼音检索码", example = "XGCJC")
    private String pyStr;

    @Size(max = 255, message = "五笔检索码不能超过255个字符")
    @Schema(description = "五笔检索码", example = "XGI")
    private String wbStr;

    @Size(max = 255, message = "销售/计价单位编码不能超过255个字符")
    @Schema(description = "销售/计价单位编码", example = "6")
    private String packageUnitCode;

    @NotBlank(message = "诊疗类型不能为空")
    @Size(max = 255, message = "诊疗类型枚举编码不能超过255个字符")
    @Schema(description = "诊疗类型枚举编码：1=检验/2=检查/3=护理/4=手术/5=其他", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Size(max = 255, message = "发布状态枚举编码不能超过255个字符")
    @Schema(description = "发布状态枚举编码：DRAFT草稿/ACTIVE启用/INACTIVE停用/DISCARDED已废弃，默认ACTIVE", example = "ACTIVE")
    private String statusEnum;

    @NotBlank(message = "分类编码不能为空")
    @Size(max = 255, message = "分类编码不能超过255个字符")
    @Schema(description = "分类编码（21=治疗/22=检验/23=检查/24=手术等，传编码值不是中文）", example = "21", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    @NotBlank(message = "业务编号不能为空")
    @Size(max = 255, message = "业务编号不能超过255个字符")
    @Schema(description = "业务编号（全局唯一）", example = "AD00000011", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Size(max = 255, message = "说明文本不能超过255个字符")
    @Schema(description = "说明文本", example = "常规检查项目")
    private String descriptionText;

    @Size(max = 255, message = "身体部位编码不能超过255个字符")
    @Schema(description = "身体部位编码", example = "BODY_BLOOD")
    private String bodySiteCode;

    @Size(max = 255, message = "标本编码不能超过255个字符")
    @Schema(description = "标本编码", example = "SPEC_BLOOD")
    private String specimenCode;

    @Schema(description = "所属机构标识", example = "1001")
    private String orgId;

    @Schema(description = "位置标识", example = "2001")
    private String locationId;

    @Schema(description = "计费规则标识", example = "1")
    private Integer ruleId;

    @Schema(description = "划价标记：0=否/1=是", example = "1")
    private Integer pricingFlag;

    @Schema(description = "医保标记：0=否/1=是", example = "0")
    private Integer ybFlag;

    @Size(max = 255, message = "医保编码不能超过255个字符")
    @Schema(description = "医保编码", example = "C010101001")
    private String ybNo;

    @Schema(description = "医保对码标记：0=未对码/1=已对码", example = "0")
    private Integer ybMatchFlag;

    @Schema(description = "医保收费等级：1=甲类/2=乙类/3=丙类(自费)", example = "1")
    private Integer chrgitmLv;

    @Schema(description = "子项标记（0=无子项/1=有子项），不传则根据children是否为空自动判断", example = "0")
    private Integer childrenFlag;

    @Size(max = 255, message = "诊疗项目业务版本不能超过255个字符")
    @Schema(description = "诊疗项目业务版本（非乐观锁 version）", example = "v1.0")
    private String activityVersion;

    @Size(max = 255, message = "商品名称不能超过255个字符")
    @Schema(description = "商品名称（可与项目名称分列维护）", example = "血常规(商品名)")
    private String merchandiseName;

    @Size(max = 255, message = "商品名拼音检索码不能超过255个字符")
    @Schema(description = "商品名拼音检索码", example = "XGCSP")
    private String merchandisePyStr;

    @Size(max = 255, message = "商品名五笔检索码不能超过255个字符")
    @Schema(description = "商品名五笔检索码", example = "XGSP")
    private String merchandiseWbStr;

    @Size(max = 255, message = "默认门诊/急诊单位编码不能超过255个字符")
    @Schema(description = "默认门诊/急诊单位编码", example = "6")
    private String defEncounterUnitCode;

    @Size(max = 255, message = "默认住院单位编码不能超过255个字符")
    @Schema(description = "默认住院单位编码", example = "6")
    private String defInhospitalUnitCode;

    // ========== 以下为关联收费表 adm_charge_item_definition 字段 ==========

    @Size(max = 255, message = "财务类型编码不能超过255个字符")
    @Schema(description = "【收费表】财务类型编码（fin_type_code字典，如1008=治疗）", example = "1008")
    private String typeCode;

    @Size(max = 255, message = "医保类别不能超过255个字符")
    @Schema(description = "【收费表】医保类别编码（med_chrgitm_type字典）", example = "1")
    private String ybType;

    @DecimalMin(value = "0.00", message = "成本价不能小于0")
    @Schema(description = "【收费表】成本价/进价（元，2位小数），不传则不写入PURCHASE价格记录", example = "3.00")
    private BigDecimal purchasePrice;

    @DecimalMin(value = "0.00", message = "零售价不能小于0")
    @Schema(description = "【收费表】零售价/销售价（元，2位小数，门诊收费用），不传则不写入SALE价格记录", example = "6.00")
    private BigDecimal retailPrice;

    @DecimalMin(value = "0.00", message = "最高零售价不能小于0")
    @Schema(description = "【收费表】最高零售价/限价（元，2位小数），不传则不写入LIMIT价格记录", example = "6.00")
    private BigDecimal maximumRetailPrice;

    // ========== 以下为关联子项子表 wkf_activity_definition_children 字段 ==========

    @Schema(description = "【子项表】子项列表，组合项目/套餐时传入，单项目传空数组[]或不传")
    private List<WkfActivityDefinitionChildVO> children;
}
