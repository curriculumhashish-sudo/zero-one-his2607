package com.zeroone.star.project.vo.j6.report;
// 患者疾病类型分布数据点：diseaseName, count

import com.zeroone.star.project.dto.j6.report.DiseaseDistributionDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：患者疾病类型分布图表数据返回对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j6小组-TKORHP
 * @version 1.0.0
 */
@Data
@Schema(description = "患者疾病类型分布图表数据返回对象")
public class DiseaseDistributionVO {
    @Schema(description = "疾病统计数据列表")
    private List<DiseaseDistributionDTO> data;
}
