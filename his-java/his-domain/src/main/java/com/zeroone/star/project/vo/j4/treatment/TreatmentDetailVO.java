package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.List;

/**
 * 【前端对接说明】
 * 1. 此VO用于诊疗项目详情页/编辑页的完整数据回显。
 * 2. 【字符串类型编码字段（需字典翻译）】：
 *    - categoryCode（目录分类）：编码对照表见下方字段注释，共30个分类。
 *    - packageUnitCode（计价单位）：编码来自单位字典表（如：6=次、7=瓶、8=袋等）。
 *    - itemTypeCode（财务类别）：编码来自财务科目字典（如：1008=治疗费等）。
 *    - bodySiteCode（身体部位）、specimenCode（标本）：编码来自对应术语字典。
 *    - statusEnum（发布状态）：1=草稿/2=已启用/3=已停用/4=已废弃。
 * 3. 【数字类型编码字段（需字典翻译）】：
 *    - typeEnum（业务类型）：1=检验/2=检查/3=护理/4=手术/5=其他。
 *    - ybFlag/pricingFlag/ybMatchFlag：布尔标志，0=否/1=是。
 *    - chrgitmLv（医保等级）：1=甲类/2=乙类/3=丙类。
 * 4. 【价格字段】：单位均为元，保留2位小数；purchasePrice=购入价，retailPrice=零售价，maximumRetailPrice=最高零售价。
 * 5. 【children字段】：当childrenFlag=1（组合项目）时，此数组包含子项列表；单项目时为空数组。
 * 6. 后端不再返回任何*_dictText翻译字段，所有字典翻译由前端根据编码对照自行维护枚举映射。
 */
@Data
@Schema(description = "诊疗项目详情显示对象")
public class TreatmentDetailVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    /**
     * 【前端需字典翻译】目录分类编码（字符串类型）
     * 编码对照表（参考后端术语表 ter_concept，code_system_id=255）：
     * 21=治疗, 22=检验, 23=检查, 24=手术, 25=麻醉, 26=护理级别, 27=膳食, 28=输血, 29=输氧,
     * 31=其他, 32=转科, 33=术后, 34=出院, 35=转院, 36=死亡, 37=产后, 38=病情, 39=护理常规,
     * 40=体位, 41=陪护, 42=隔离等级, 43=口腔检查, 44=内镜检查, 45=内镜治疗, 46=基础服务,
     * 47=诊断费, 48=预防接种手续费, 49=口腔治疗, 50=护理项目, 51=床位服务
     */
    @Schema(description = "目录分类编码（字符串类型），前端需字典翻译：21=治疗/22=检验/23=检查/24=手术/25=麻醉/26=护理级别/27=膳食/28=输血/29=输氧/31=其他/32=转科/33=术后/34=出院/35=转院/36=死亡/37=产后/38=病情/39=护理常规/40=体位/41=陪护/42=隔离等级/43=口腔检查/44=内镜检查/45=内镜治疗/46=基础服务/47=诊断费/48=预防接种手续费/49=口腔治疗/50=护理项目/51=床位服务", example = "21")
    private String categoryCode;

    @Schema(description = "业务编号", example = "AD00000011")
    private String busNo;

    @Schema(description = "项目名称", example = "静脉注射")
    private String name;

    @Schema(description = "拼音检索码", example = "JMZS")
    private String pyStr;

    @Schema(description = "五笔检索码", example = "GEIT")
    private String wbStr;

    @Schema(description = "销售/计价单位编码（字符串类型），前端需字典翻译：来自单位字典表", example = "6")
    private String packageUnitCode;

    @Schema(description = "诊疗业务版本", example = "V1.0")
    private String activityVersion;

    @Schema(description = "商品名称", example = "静脉注射套餐")
    private String merchandiseName;

    @Schema(description = "商品名拼音检索码", example = "JMZSTC")
    private String merchandisePyStr;

    @Schema(description = "商品名五笔检索码", example = "GEITDC")
    private String merchandiseWbStr;

    @Schema(description = "默认门诊单位编码", example = "6")
    private String defEncounterUnitCode;

    @Schema(description = "默认住院单位编码", example = "6")
    private String defInhospitalUnitCode;

    @Schema(description = "医保标记（数字类型），0=否/1=是", example = "0")
    private Integer ybFlag;

    @Schema(description = "划价标记（数字类型），0=否/1=是", example = "0")
    private Integer pricingFlag;

    @Schema(description = "医保编码", example = "")
    private String ybNo;

    @Schema(description = "医保对码标记（数字类型），0=未对码/1=已对码", example = "0")
    private Integer ybMatchFlag;

    /**
     * 【前端需字典翻译】发布状态（数字类型）
     * 对应后端枚举 PublicationStatus（返回sortOrder值）：
     * 1=草稿(DRAFT), 2=已启用(ACTIVE), 3=已停用(INACTIVE), 4=已废弃(DISCARDED)
     */
    @Schema(description = "发布状态（数字类型），前端需字典翻译：1=草稿/2=已启用/3=已停用/4=已废弃", example = "2")
    private Integer statusEnum;

    @Schema(description = "身体部位编码（字符串类型），前端需字典翻译", example = "")
    private String bodySiteCode;

    @Schema(description = "标本编码（字符串类型），前端需字典翻译", example = "")
    private String specimenCode;

    @Schema(description = "说明文本", example = "")
    private String descriptionText;

    @Schema(description = "计费规则ID", example = "1")
    private Integer ruleId;

    @Schema(description = "删除标志（数字类型），0=未删除/1=已删除", example = "0")
    private Integer deleteFlag;

    /**
     * 【前端需字典翻译】业务类型（数字类型）
     * 对应后端枚举 ActivityType：
     * 1=检验(PROOF), 2=检查(TEST), 3=护理(CARE), 4=手术(OPERATION), 5=其他(OTHER)
     */
    @Schema(description = "业务类型（数字类型），前端需字典翻译：1=检验/2=检查/3=护理/4=手术/5=其他", example = "5")
    private Integer typeEnum;

    @Schema(description = "所属机构ID", example = "")
    private String orgId;

    @Schema(description = "所在位置ID", example = "")
    private String locationId;

    @Schema(description = "收费项目医保等级（数字类型），前端需字典翻译：1=甲类/2=乙类/3=丙类", example = "")
    private Integer chrgitmLv;

    @Schema(description = "医保类别编码（字符串类型），前端需字典翻译", example = "")
    private String ybType;

    @Schema(description = "财务类别编码（字符串类型），前端需字典翻译：来自财务科目字典", example = "1008")
    private String itemTypeCode;

    @Schema(description = "项目购入价（单位：元，保留2位小数）", example = "6.00")
    private BigDecimal purchasePrice;

    @Schema(description = "项目零售价（单位：元，保留2位小数）", example = "6.00")
    private BigDecimal retailPrice;

    @Schema(description = "项目最高零售价（单位：元，保留2位小数）", example = "6.00")
    private BigDecimal maximumRetailPrice;

    @Schema(description = "诊疗子项列表（组合项目时返回，单项目为空数组）")
    private List<WkfActivityDefinitionChildVO> children;
}
