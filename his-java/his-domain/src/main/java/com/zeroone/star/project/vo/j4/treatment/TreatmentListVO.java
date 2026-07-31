package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * 【前端对接说明】
 * 1. 此VO用于诊疗项目管理模块的列表分页查询结果展示。
 * 2. categoryCode为目录分类编码（字符串类型），编码对照表见TreatmentDetailVO类注释。
 * 3. typeEnum为业务类型（数字类型），对应枚举：1=检验/2=检查/3=护理/4=手术/5=其他。
 * 4. statusEnum为发布状态（数字类型），对应枚举：1=草稿/2=已启用/3=已停用/4=已废弃。
 * 5. price为零售价（SALE类型价格），单位：元，保留2位小数。
 * 6. 前端应复用TreatmentDetailVO中定义的字典映射，不要单独维护重复字典。
 */
@Data
@Schema(description = "诊疗项目列表显示对象")
public class TreatmentListVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键(编码)", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "项目名称", example = "静脉注射", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "发布状态（数字类型），前端需字典翻译：1=草稿/2=已启用/3=已停用/4=已废弃", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer statusEnum;

    @Schema(description = "业务类型（数字类型），前端需字典翻译：1=检验/2=检查/3=护理/4=手术/5=其他", example = "5", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer typeEnum;

    @Schema(description = "零售价(元)", example = "6.00")
    private BigDecimal price;
}
