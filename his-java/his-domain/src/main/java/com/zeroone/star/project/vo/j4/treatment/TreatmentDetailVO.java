package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.List;

/**
 * 【前端对接说明 - 请仔细阅读】
 *
 * === 一、接口用途 ===
 * 本VO用于「诊疗项目详情页/编辑页」数据回显，对应 GET /treatment/get-treatment?id=xxx 接口。
 *
 * === 二、返回字段与前端表单对照 ===
 *
 * 【基础信息区域】
 * ┌──────────────┬──────────────────┬──────────┬─────────────────────────────────────────┐
 * │ 前端标签      │ 字段名            │ 是否必填 │ 控件类型 / 数据源                        │
 * ├──────────────┼──────────────────┼──────────┼─────────────────────────────────────────┤
 * │ （隐藏）      │ id               │ 是       │ 隐藏字段，新增不传，编辑/详情必传         │
 * │ （只读展示）  │ busNo            │ 只读     │ 文本展示，系统自动生成业务编号            │
 * │ *诊疗名称     │ name             │ 是       │ 文本输入框                              │
 * │ *目录分类     │ categoryCode     │ 是       │ 下拉框，字典接口（activity_category_code）│
 * │ *业务类型     │ typeEnum         │ 是       │ 下拉框，固定枚举（1-5），前端写死即可     │
 * │ 销售/计价单位 │ packageUnitCode  │ 否       │ 下拉框，字典接口（unit_code）            │
 * │ 拼音         │ pyStr            │ 否       │ 文本输入框（可由名称自动生成）            │
 * │ 五笔         │ wbStr            │ 否       │ 文本输入框（可由名称自动生成）            │
 * │ 财务类型      │ typeCode        │ 否       │ 下拉框，字典接口（fin_type_code）        │
 * │ 成本价        │ purchasePrice    │ 否       │ 数字输入框，单位元，保留2位小数           │
 * │ 零售价        │ retailPrice      │ 否       │ 数字输入框，单位元，保留2位小数           │
 * │ 最高零售价    │ maximumRetailPrice│ 否      │ 数字输入框，单位元，保留2位小数           │
 * │ 描述         │ descriptionText  │ 否       │ 多行文本域                              │
 * └──────────────┴──────────────────┴──────────┴─────────────────────────────────────────┘
 *
 * 【医保信息区域】
 * ┌──────────────┬──────────────────┬──────────┬─────────────────────────────────────────┐
 * │ 前端标签      │ 字段名            │ 是否必填 │ 控件类型 / 数据源                        │
 * ├──────────────┼──────────────────┼──────────┼─────────────────────────────────────────┤
 * │ 医保标志      │ ybFlag           │ 否       │ 单选框（0=否 / 1=是），默认否             │
 * │ 医保编号      │ ybNo             │ 否       │ 文本输入框，ybFlag=1时才需要填            │
 * │ 医保类别      │ ybType           │ 否       │ 下拉框，字典接口（med_chrgitm_type）     │
 * │ 医保对码标记  │ ybMatchFlag      │ 否       │ 单选框（0=未对码 / 1=已对码），默认0      │
 * │ 医保等级      │ chrgitmLv        │ 否       │ 下拉框，字典接口（chrgitm_lv：1甲/2乙/3丙）│
 * └──────────────┴──────────────────┴──────────┴─────────────────────────────────────────┘
 *
 * 【子项区域】
 * ┌──────────────┬──────────────────┬──────────┬─────────────────────────────────────────┐
 * │ 前端标签      │ 字段名            │ 是否必填 │ 控件类型 / 数据源                        │
 * ├──────────────┼──────────────────┼──────────┼─────────────────────────────────────────┤
 * │ 子项列表      │ children         │ 否       │ 动态列表，点击"添加子项"弹出选择诊疗弹窗   │
 * └──────────────┴──────────────────┴──────────┴─────────────────────────────────────────┘
 *
 * 【状态字段（详情展示，编辑不可改）】
 * ┌──────────────┬──────────────────┬─────────────────────────────────────────────────┐
 * │ 字段名        │ 含义              │ 说明                                            │
 * ├──────────────┼──────────────────┼─────────────────────────────────────────────────┤
 * │ statusEnum   │ 发布状态          │ 字符串枚举：DRAFT=草稿/ACTIVE=已启用/INACTIVE=已停用/DISCARDED=已废弃 │
 * └──────────────┴──────────────────┴─────────────────────────────────────────────────┘
 *
 * === 三、数据类型与翻译规则 ===
 * 1. 【固定枚举字段（String）】：前端写死枚举映射即可，不需要调字典接口，数据库存储原始字符串。
 *    - typeEnum: "1"=检验(PROOF),"2"=检查(TEST),"3"=护理(CARE),"4"=手术(OPERATION),"5"=其他(OTHER)
 *    - statusEnum: "DRAFT"=草稿,"ACTIVE"=已启用,"INACTIVE"=已停用,"DISCARDED"=已废弃
 *    - ybFlag/ybMatchFlag: 0=否,1=是（Integer类型，与数据库tinyint一致）
 *    - chrgitmLv: 1=甲类,2=乙类,3=丙类(自费)（Integer类型，与数据库tinyint一致）
 *
 * 2. 【字典编码字段（String）】：前端必须通过字典接口动态获取选项，不要写死。
 *    - categoryCode: 字典code_system = activity_category_code（目录分类）
 *    - packageUnitCode: 字典code_system = unit_code（销售/计价单位）
 *    - typeCode: 字典code_system = fin_type_code（财务类型）
 *    - ybType: 字典code_system = med_chrgitm_type（医保类别）
 *
 * 3. 【价格字段（BigDecimal）】：单位人民币"元"，保留2位小数，展示格式化为 ¥xx.xx。
 *    purchasePrice=成本价, retailPrice=零售价, maximumRetailPrice=最高零售价
 *
 * 4. 后端不再返回任何 *_dictText 中文翻译字段！所有字典翻译由前端自行完成。
 *
 * === 四、子项数据说明 ===
 * - children 数组仅当该项目为"组合项目"时有数据；单项目时返回空数组 []。
 * - 每个子项字段说明见 WkfActivityDefinitionChildVO 类注释。
 *
 * @author j4组-treatment模块
 * @version 1.0.0
 */
@Data
@Schema(description = "诊疗项目详情显示对象（用于详情页/编辑页数据回显）")
public class TreatmentDetailVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID（隐藏字段，系统生成，新增不传，编辑/详情必传）", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务编号/编码（只读展示，系统自动生成，如AD00000011）", example = "AD00000011")
    private String busNo;

    @Schema(description = "【必填·文本框】诊疗项目名称", example = "静脉注射", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 【前端需字典翻译·下拉框数据源: 字典接口activity_category_code】
     * 目录分类编码（字符串类型）
     * 常用编码参考（最终以字典接口返回为准）：
     * 21=治疗, 22=检验, 23=检查, 24=手术, 25=麻醉, 26=护理级别, 27=膳食, 28=输血, 29=输氧,
     * 31=其他, 32=转科, 33=术后, 34=出院, 35=转院, 36=死亡, 37=产后, 38=病情, 39=护理常规,
     * 40=体位, 41=陪护, 42=隔离等级, 43=口腔检查, 44=内镜检查, 45=内镜治疗, 46=基础服务,
     * 47=诊断费, 48=预防接种手续费, 49=口腔治疗, 50=护理项目, 51=床位服务
     */
    @Schema(description = "【必填·下拉框】目录分类编码，字典数据源activity_category_code，常用值：21=治疗/22=检验/23=检查/24=手术/25=麻醉/31=其他", example = "21", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    /**
     * 【前端需字典翻译·固定枚举，前端写死即可】
     * 业务类型（字符串类型，与数据库一致）：
     * 1=检验(PROOF) - 如血常规、尿常规等实验室检验项目
     * 2=检查(TEST) - 如CT、X光、B超等影像/功能检查项目
     * 3=护理(CARE) - 如一级护理、口腔护理等护理操作
     * 4=手术(OPERATION) - 各类手术项目
     * 5=其他(OTHER) - 如静脉注射、换药等治疗类及其他项目
     */
    @Schema(description = "【必填·下拉框·枚举】业务类型（字符串）：1=检验/2=检查/3=护理/4=手术/5=其他", example = "5", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    /**
     * 【前端需字典翻译·下拉框数据源: 字典接口unit_code】
     * 销售/计价单位编码（字符串类型）
     * 常用编码参考（最终以字典接口返回为准）：
     * 1=片, 2=盒, 3=ml, 4=mg, 5=粒, 6=次, 7=箱, 8=瓶, 9=支, 10=g, 11=袋, 14=日, 71=床日
     */
    @Schema(description = "【下拉框】销售/计价单位编码，字典数据源unit_code，常用值：6=次/8=瓶/14=日/71=床日", example = "6")
    private String packageUnitCode;

    @Schema(description = "【文本框】拼音检索码（可由名称自动生成，支持手动修改）", example = "JMZS")
    private String pyStr;

    @Schema(description = "【文本框】五笔检索码（可由名称自动生成，支持手动修改）", example = "GEIT")
    private String wbStr;

    /**
     * 【前端需字典翻译·下拉框数据源: 字典接口fin_type_code】
     * 财务类别编码（字符串类型）
     * 常用编码参考（最终以字典接口返回为准）：
     * 1001=西药, 1002=外购药品, 1003=中草药, 1004=化验, 1005=手术, 1007=诊断,
     * 1008=治疗, 1009=护理, 1010=床位, 1011=挂号费, 1012=诊察费, 1013=其他费,
     * 2001=麻醉, 2002=膳食, 2003=输血费, 2005=医用耗材, 2009=检查, 2010=中成药
     */
    @Schema(description = "【下拉框】财务类别编码，字典数据源fin_type_code，常用值：1008=治疗费/1009=护理费/1013=其他费", example = "1008")
    private String typeCode;

    @Schema(description = "【数字输入框】成本价/购入价（单位：元，保留2位小数）", example = "3.00")
    private BigDecimal purchasePrice;

    @Schema(description = "【数字输入框】零售价/销售价（单位：元，保留2位小数，门诊收费使用此价格）", example = "6.00")
    private BigDecimal retailPrice;

    @Schema(description = "【数字输入框】最高零售价/限价（单位：元，保留2位小数，为物价部门限价）", example = "6.00")
    private BigDecimal maximumRetailPrice;

    @Schema(description = "【多行文本域】说明/描述文本", example = "")
    private String descriptionText;

    @Schema(description = "【单选框】医保标志，0=否/1=是，默认0", example = "0")
    private Integer ybFlag;

    @Schema(description = "【文本框·ybFlag=1时显示】医保编号/医保编码", example = "")
    private String ybNo;

    /**
     * 【前端需字典翻译·下拉框数据源: 字典接口med_chrgitm_type】
     * 医保类别编码（字符串类型）
     * 常用编码参考（最终以字典接口返回为准）：
     * 1=床位费, 2=诊察费, 3=检查费, 4=化验费, 5=治疗费, 6=手术费, 7=护理费,
     * 8=卫生材料费, 9=西药费, 10=中药饮片费, 11=中成药费, 12=一般诊疗费, 13=挂号费, 14=其他费
     */
    @Schema(description = "【下拉框】医保类别编码，字典数据源med_chrgitm_type，常用值：1=床位费/5=治疗费/9=西药费/14=其他费", example = "")
    private String ybType;

    @Schema(description = "【单选框】医保对码标记，0=未对码/1=已对码，默认0", example = "0")
    private Integer ybMatchFlag;

    /**
     * 【前端需字典翻译·下拉框数据源: 字典接口chrgitm_lv】
     * 医保等级（数字类型）：
     * 1=甲类 - 医保全额报销
     * 2=乙类 - 医保部分报销，个人自付一部分
     * 3=丙类/自费 - 医保不报销，完全自费
     */
    @Schema(description = "【下拉框】医保等级（数字）：1=甲类/2=乙类/3=丙类(自费)，字典数据源chrgitm_lv", example = "")
    private Integer chrgitmLv;

    /**
     * 【前端需字典翻译·固定枚举，前端写死即可】
     * 发布状态（字符串类型，与数据库一致）：
     * DRAFT=草稿 - 新建未发布
     * ACTIVE=已启用 - 正常使用中
     * INACTIVE=已停用 - 暂时停用
     * DISCARDED=已废弃 - 永久废弃
     */
    @Schema(description = "发布状态（只读·字符串枚举）：DRAFT=草稿/ACTIVE=已启用/INACTIVE=已停用/DISCARDED=已废弃", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "【子项动态列表】诊疗子项列表，组合项目时返回子项数组，单项目返回空数组[]。每个元素字段见WkfActivityDefinitionChildVO")
    private List<WkfActivityDefinitionChildVO> children;
}
