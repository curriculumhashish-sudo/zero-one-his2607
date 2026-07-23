package com.zeroone.star.project.query.j4.treatment;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@EqualsAndHashCode(callSuper = true)
@Data
@Schema(description = "诊疗项目分页查询对象")
public class TreatmentQuery extends PageQuery {

    @Schema(description = "关键词（诊疗名称或编码，模糊查询）", example = "CT")
    //可为空
    private String keyword;

    @Schema(description = "目录分类编码", example = " \"检验\", \"检查\", \"治疗\", \"手术\", \"麻醉\", \"护理级别\", \"护理项目\", \"膳食\", \"输血\", \"输氧\",\n" +
            "            \"其他\", \"转科\", \"术后\", \"出院\", \"转院\", \"死亡\", \"产后\", \"病情\", \"护理常规\", \"体位\",\n" +
            "            \"陪护\", \"隔离等级\", \"口腔检查\", \"内镜检查\", \"内镜治疗\", \"床位服务\", \"基础服务\", \"诊断费\",\n" +
            "            \"预防接种手续费\", \"口腔治疗\"")
    /** 诊疗大类/分类编码过滤；空表示不按分类过滤。 */
    private String categoryCode;
}
