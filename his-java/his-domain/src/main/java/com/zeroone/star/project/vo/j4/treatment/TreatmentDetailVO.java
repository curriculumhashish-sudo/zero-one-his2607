package com.zeroone.star.project.vo.j4.treatment;

import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.fasterxml.jackson.databind.ser.std.ToStringSerializer;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.List;

/**
 * 诊疗项目详情显示对象
 *
 * 【前端对接说明】
 * 1. 本对象所有编码类字段（后缀为Code或Enum的字段）后端仅返回原始编码值，不返回中文翻译文本
 * 2. 前端需根据下方各字段注释中的「编码对照表」自行维护枚举/字典映射，在展示时转换为中文
 * 3. 建议前端统一封装字典翻译工具方法，根据字段名+编码值获取对应中文
 * 4. 价格字段单位为「元」，保留2位小数
 * 5. children字段为子项列表，当childrenFlag=1时有值，否则为空数组
 */
@Data
@Schema(description = "诊疗项目详情显示对象")
public class TreatmentDetailVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @JsonSerialize(using = ToStringSerializer.class)
    @Schema(description = "主键ID", example = "1890000000000000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    /**
     * 【前端需字典翻译】目录分类编码
     * 后端返回编码值，前端需根据编码维护枚举/字典映射为中文显示，不直接显示编码。
     * 编码对照表（参考后端术语表 ter_concept，code_system_id=255）：
     * 21=治疗, 22=检验, 23=检查, 24=手术, 25=麻醉, 26=护理级别, 27=膳食, 28=输血, 29=输氧,
     * 31=其他, 32=转科, 33=术后, 34=出院, 35=转院, 36=死亡, 37=产后, 38=病情, 39=护理常规,
     * 40=体位, 41=陪护, 42=隔离等级, 43=口腔检查, 44=内镜检查, 45=内镜治疗, 46=基础服务,
     * 47=诊断费, 48=预防接种手续费, 49=口腔治疗, 50=护理项目, 51=床位服务
     */
    @Schema(description = "目录分类编码，前端需字典翻译：21=治疗/22=检验/23=检查/24=手术/25=麻醉/26=护理级别/27=膳食/28=输血/29=输氧/31=其他/32=转科/33=术后/34=出院/35=转院/36=死亡/37=产后/38=病情/39=护理常规/40=体位/41=陪护/42=隔离等级/43=口腔检查/44=内镜检查/45=内镜治疗/46=基础服务/47=诊断费/48=预防接种手续费/49=口腔治疗/50=护理项目/51=床位服务", example = "22")
    private String categoryCode;

    @Schema(description = "业务编码", example = "AD00000001")
    private String busNo;

    @Schema(description = "项目名称", example = "肝功13项")
    private String name;

    @Schema(description = "拼音检索码", example = "gg13x")
    private String pyStr;

    @Schema(description = "五笔检索码", example = "af13c")
    private String wbStr;

    /**
     * 【前端需字典翻译】销售/计价单位编码
     * 后端返回编码值，前端需维护枚举映射为中文显示。
     * 编码对照表（参考后端术语表 ter_concept，code_system_id=279125561775755265）：
     * 1=片, 2=盒, 3=ml, 4=mg, 5=粒, 6=次, 7=箱, 8=瓶, 9=支, 10=g, 11=袋, 14=日, 71=床日
     */
    @Schema(description = "销售/计价单位编码，前端需字典翻译：1=片/2=盒/3=ml/4=mg/5=粒/6=次/7=箱/8=瓶/9=支/10=g/11=袋/14=日/71=床日", example = "6")
    private String packageUnitCode;

    @Schema(description = "诊疗业务版本", example = "V1.0")
    private String activityVersion;

    @Schema(description = "商品名称", example = "肝功13项套餐")
    private String merchandiseName;

    @Schema(description = "商品名拼音检索码", example = "gg13xtc")
    private String merchandisePyStr;

    @Schema(description = "商品名五笔检索码", example = "af13wdc")
    private String merchandiseWbStr;

    /**
     * 【前端需字典翻译】默认门诊单位编码，同 packageUnitCode 编码对照表
     */
    @Schema(description = "默认门诊单位编码，前端需字典翻译：1=片/2=盒/3=ml/4=mg/5=粒/6=次/7=箱/8=瓶/9=支/10=g/11=袋/14=日/71=床日", example = "6")
    private String defEncounterUnitCode;

    /**
     * 【前端需字典翻译】默认住院单位编码，同 packageUnitCode 编码对照表
     */
    @Schema(description = "默认住院单位编码，前端需字典翻译：1=片/2=盒/3=ml/4=mg/5=粒/6=次/7=箱/8=瓶/9=支/10=g/11=袋/14=日/71=床日", example = "6")
    private String defInhospitalUnitCode;

    @Schema(description = "医保标记 0-否 1-是", example = "1")
    private Integer ybFlag;

    @Schema(description = "划价标记 0-否 1-是", example = "1")
    private Integer pricingFlag;

    @Schema(description = "医保编码", example = "C010101001")
    private String ybNo;

    @Schema(description = "医保对码标记 0-未对码 1-已对码", example = "1")
    private Integer ybMatchFlag;

    /**
     * 【前端需字典翻译】发布状态
     * 后端返回字符串枚举编码，前端需维护枚举映射为中文显示。
     * 编码对照表（参考后端枚举 PublicationStatus）：
     * DRAFT=草稿, ACTIVE=已启用, INACTIVE=已停用, DISCARDED=已废弃
     */
    @Schema(description = "发布状态，前端需字典翻译：DRAFT=草稿/ACTIVE=已启用/INACTIVE=已停用/DISCARDED=已废弃", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "身体部位编码", example = "ARM_VEIN")
    private String bodySiteCode;

    @Schema(description = "标本编码", example = "BLOOD")
    private String specimenCode;

    @Schema(description = "说明文本", example = "常规肝功能检测项目")
    private String descriptionText;

    @Schema(description = "计费规则ID", example = "1")
    private Integer ruleId;

    @Schema(description = "删除标志 0-未删除 1-已删除", example = "0")
    private Integer deleteFlag;

    /**
     * 【前端需字典翻译】业务类型编码
     * 后端返回字符串编码，前端需维护枚举映射为中文显示。
     * 编码对照表（参考后端枚举 ActivityType）：
     * 1=检验, 2=检查, 3=护理, 4=手术, 5=其他
     */
    @Schema(description = "业务类型编码，前端需字典翻译：1=检验/2=检查/3=护理/4=手术/5=其他", example = "1")
    private String typeEnum;

    @Schema(description = "所属机构ID", example = "1001")
    private String orgId;

    @Schema(description = "所在位置ID", example = "2001")
    private String locationId;

    @Schema(description = "收费项目医保等级（1-甲类/2-乙类/3-丙类）", example = "1")
    private Integer chrgitmLv;

    @Schema(description = "允许的角色枚举值", example = "1")
    private Integer permittedRoleEnum;

    /**
     * 【前端需字典翻译】医保类别编码
     * 编码对照表（参考后端术语表 ter_concept，code_system_id=254）：
     * 1=自费, 2=医保
     */
    @Schema(description = "医保类别编码，前端需字典翻译：1=自费/2=医保", example = "1")
    private String ybType;

    /**
     * 【前端需字典翻译】财务类别编码
     * 后端返回编码值，前端需维护枚举映射为中文显示。
     * 编码对照表（参考后端术语表 ter_concept，code_system_id=236）：
     * 1001=西药, 1002=外购药品, 1003=中草药, 1004=化验, 1005=手术, 1007=诊断, 1008=治疗,
     * 1009=护理, 1010=床位, 1011=挂号费, 1012=诊察费, 1013=其他费, 2001=麻醉, 2002=膳食,
     * 2003=输血费, 2004=自备医嘱, 2005=医用耗材, 2006=循环物质, 2007=物资, 2008=办公用品,
     * 2009=检查, 2010=中成药, 2011=CT, 2012=电诊
     */
    @Schema(description = "财务类别编码，前端需字典翻译：1001=西药/1002=外购药品/1003=中草药/1004=化验/1005=手术/1007=诊断/1008=治疗/1009=护理/1010=床位/1011=挂号费/1012=诊察费/1013=其他费/2001=麻醉/2002=膳食/2003=输血费/2004=自备医嘱/2005=医用耗材/2006=循环物质/2007=物资/2008=办公用品/2009=检查/2010=中成药/2011=CT/2012=电诊", example = "1008")
    private String itemTypeCode;

    @Schema(description = "项目购入价（元）", example = "44.40")
    private BigDecimal purchasePrice;

    @Schema(description = "项目零售价（元）", example = "44.40")
    private BigDecimal retailPrice;

    @Schema(description = "项目最高零售价（元）", example = "44.40")
    private BigDecimal maximumRetailPrice;

    @Schema(description = "诊疗子项列表")
    private List<WkfActivityDefinitionChildVO> children;
}
