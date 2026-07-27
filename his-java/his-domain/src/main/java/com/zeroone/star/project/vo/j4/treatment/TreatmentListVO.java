package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * 诊疗项目列表显示对象
 *
 * 【前端对接说明】
 * 1. 本对象用于诊疗项目管理页面的列表展示，包含列表页所需的核心字段
 * 2. 编码类字段（categoryCode/statusEnum/typeEnum）后端仅返回原始编码值，前端需自行字典翻译
 * 3. 编码对照表与TreatmentDetailVO保持一致，建议前端复用同一套字典映射
 * 4. price字段为项目零售价，单位为「元」，保留2位小数
 */
@Data
@Schema(description = "诊疗项目列表显示对象")
public class TreatmentListVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键(编码)", example = "123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "项目名称", example = "血常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "业务编码", example = "AD00000001")
    private String busNo;

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

    /**
     * 【前端需字典翻译】发布状态
     * 后端返回字符串枚举编码，前端需维护枚举映射为中文显示。
     * 编码对照表（参考后端枚举 PublicationStatus）：
     * DRAFT=草稿, ACTIVE=已启用, INACTIVE=已停用, DISCARDED=已废弃
     */
    @Schema(description = "发布状态，前端需字典翻译：DRAFT=草稿/ACTIVE=已启用/INACTIVE=已停用/DISCARDED=已废弃", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    /**
     * 【前端需字典翻译】业务类型编码
     * 后端返回字符串编码，前端需维护枚举映射为中文显示。
     * 编码对照表（参考后端枚举 ActivityType）：
     * 1=检验, 2=检查, 3=护理, 4=手术, 5=其他
     */
    @Schema(description = "业务类型编码，前端需字典翻译：1=检验/2=检查/3=护理/4=手术/5=其他", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "零售价格(元)", example = "35.00")
    private BigDecimal price;
}
