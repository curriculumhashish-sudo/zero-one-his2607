package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 【前端对接说明】
 * 1. 此VO用于其他业务模块（如门诊医生站、收费等）选择诊疗项目时的下拉/分页选择组件。
 * 2. categoryCode为目录分类编码（字符串类型），前端需自行维护字典映射；编码对照表见TreatmentDetailVO。
 * 3. typeEnum为业务类型（数字类型），对应枚举：1=检验/2=检查/3=护理/4=手术/5=其他。
 * 4. 前端应复用TreatmentDetailVO中定义的字典映射，不要单独维护重复字典。
 * 5. 仅返回启用状态（statusEnum=ACTIVE）的诊疗项目供选择。
 */
@Data
@Schema(description = "诊疗项目选择显示对象（其他业务选择诊疗时使用）")
public class TreatmentSelectVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键(编码)", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务编码", example = "AD00000011", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "项目名称", example = "静脉注射", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "目录分类编码（字符串类型），前端需字典翻译：21=治疗/22=检验/23=检查/24=手术/25=麻醉/26=护理级别/27=膳食/28=输血/29=输氧/31=其他/32=转科/33=术后/34=出院/35=转院/36=死亡/37=产后/38=病情/39=护理常规/40=体位/41=陪护/42=隔离等级/43=口腔检查/44=内镜检查/45=内镜治疗/46=基础服务/47=诊断费/48=预防接种手续费/49=口腔治疗/50=护理项目/51=床位服务", example = "21", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    @Schema(description = "业务类型（数字类型），前端需字典翻译：1=检验/2=检查/3=护理/4=手术/5=其他", example = "5", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer typeEnum;
}
