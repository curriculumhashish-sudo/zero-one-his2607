package com.zeroone.star.project.query.j5.configuration_management.medical_record_template;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：获取文书类型列表数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author yjyl
 * @version 1.0.0
 */
@Schema(description = "文书类型分页查询条件")
@Data
public class EmrDocumentTypeQuery extends PageQuery{

    @Schema(description = "搜索关键字（匹配编码/名称）", example = "门诊", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String keyword;

}
