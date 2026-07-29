package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * 诊疗项目选择显示对象
 *
 * 【前端对接说明】
 * 1. 本对象用于其他业务模块（如处方、医嘱等）选择诊疗项目时的下拉选择/弹窗选择组件
 * 2. 仅包含选择时需要展示的核心字段：ID、编码、名称、分类、价格
 * 3. categoryCode为目录分类编码，前端如需展示分类名称需自行字典翻译
 * 4. 编码对照表与TreatmentDetailVO/TreatmentListVO保持一致，建议前端复用同一套字典映射
 * 5. retailPrice字段为零售价，单位为「元」，保留2位小数
 */
@Data
@Schema(description = "诊疗项目选择显示对象（其他业务选择诊疗时使用）")
public class TreatmentSelectVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键(编码)", example = "123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务编码", example = "AD00000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "项目名称", example = "血常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 【前端需字典翻译】目录分类编码
     * 后端返回编码值，前端需根据编码维护枚举/字典映射为中文显示。
     * 编码对照表（参考后端术语表 ter_concept，code_system_id=255）：
     * 21=治疗, 22=检验, 23=检查, 24=手术, 25=麻醉, 26=护理级别, 27=膳食, 28=输血, 29=输氧,
     * 31=其他, 32=转科, 33=术后, 34=出院, 35=转院, 36=死亡, 37=产后, 38=病情, 39=护理常规,
     * 40=体位, 41=陪护, 42=隔离等级, 43=口腔检查, 44=内镜检查, 45=内镜治疗, 46=基础服务,
     * 47=诊断费, 48=预防接种手续费, 49=口腔治疗, 50=护理项目, 51=床位服务
     */
    @Schema(description = "目录分类编码，前端需字典翻译：21=治疗/22=检验/23=检查/24=手术/25=麻醉/26=护理级别/27=膳食/28=输血/29=输氧/31=其他/32=转科/33=术后/34=出院/35=转院/36=死亡/37=产后/38=病情/39=护理常规/40=体位/41=陪护/42=隔离等级/43=口腔检查/44=内镜检查/45=内镜治疗/46=基础服务/47=诊断费/48=预防接种手续费/49=口腔治疗/50=护理项目/51=床位服务", example = "22", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    @Schema(description = "零售价格(元)", example = "35.00")
    private BigDecimal retailPrice;
}
