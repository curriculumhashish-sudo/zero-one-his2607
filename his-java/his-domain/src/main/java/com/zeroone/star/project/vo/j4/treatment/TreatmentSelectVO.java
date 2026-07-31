package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 【前端对接说明 - 诊疗项目选择器】
 *
 * === 用途 ===
 * 本VO用于「其他业务模块选择诊疗项目」场景的分页选择组件数据，对应 GET /treatment/query-select-treatment-list 接口。
 * 典型使用场景：
 *   1. 新增/编辑诊疗组合项目时，点击「+ 添加子项」弹出的选择弹窗
 *   2. 门诊医生站开处方/医嘱时选择诊疗项目
 *   3. 收费模块录入诊疗费用时选择项目
 *
 * === 选择弹窗列表列 ===
 * ┌──────────┬──────────────┬──────────────────────────────────────────────────────┐
 * │ 弹窗列名  │ 字段名        │ 说明                                                 │
 * ├──────────┼──────────────┼──────────────────────────────────────────────────────┤
 * │ 编码      │ busNo         │ 业务编码，如AD00000011，辅助识别                      │
 * │ 名称      │ name          │ 项目名称，主选择列                                    │
 * │ 分类      │ categoryCode  │ 目录分类编码，字典翻译展示中文分类名                   │
 * │ 类型      │ typeEnum      │ 业务类型（数字枚举）：1=检验/2=检查/3=护理/4=手术/5=其他│
 * └──────────┴──────────────┴──────────────────────────────────────────────────────┘
 *
 * === 交互说明 ===
 * 1. 本接口只返回【已启用】(statusEnum=2) 的诊疗项目，草稿/停用/废弃状态的项目不会出现在选择列表中。
 * 2. 支持关键词搜索（匹配名称/编码/拼音/五笔）和目录分类筛选。
 * 3. 选中某一行后：
 *    - 将 id 作为 childrenId（子项场景）或相关引用ID
 *    - 将 name 作为显示名称/childrenName
 *    - 将 busNo 作为辅助展示编码（可选）
 * 4. 字典映射复用 TreatmentDetailVO 中的规则，不要单独维护。
 *
 * @author j4组-treatment模块
 * @version 1.0.0
 */
@Data
@Schema(description = "诊疗项目选择显示对象（供其他业务模块选择诊疗项目时使用，仅返回已启用项目）")
public class TreatmentSelectVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "诊疗项目主键ID（选中后作为引用ID传回，如子项childrenId）", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务编码/项目编码（如AD00000011，辅助识别列）", example = "AD00000011", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "诊疗项目名称（主选择列，点击选中后作为显示名）", example = "静脉注射", requiredMode = Schema.RequiredMode.REQUIRED)
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
    @Schema(description = "【需翻译】目录分类编码，字典数据源activity_category_code，常用值：21=治疗/22=检验/23=检查/31=其他", example = "21", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    /**
     * 【前端需字典翻译·固定枚举】
     * 业务类型（数字类型）：1=检验/2=检查/3=护理/4=手术/5=其他
     */
    @Schema(description = "【需翻译·枚举】业务类型（数字）：1=检验/2=检查/3=护理/4=手术/5=其他", example = "5")
    private Integer typeEnum;
}
