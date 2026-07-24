package com.zeroone.star.project.dto.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：患者疾病类型分布数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j6小组-TKORHP
 * @version 1.0.0
 */
@Data
@Schema(description = "患者疾病类型分布数据传输对象")
public class DiseaseDistributionDTO {
    /*感冒呼吸道疾病、高血压、糖尿病、肠胃疾病、骨关节病、其他病症*/
    @Schema(description = "疾病名称", example = "高血压")
    private String diseaseName;

    @Schema(description = "就诊人数", example = "215")
    private Integer count;

    @Schema(description = "占比（百分比）", example = "21.5%")
    private Double percentage;
}
