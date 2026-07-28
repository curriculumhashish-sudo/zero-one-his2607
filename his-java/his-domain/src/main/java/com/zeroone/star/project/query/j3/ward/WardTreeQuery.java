package com.zeroone.star.project.query.j3.ward;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：根据条件机构名称获取病区/病房/床位列表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhu
 * @version 1.0.0
 */

@Data
@Schema(description = "病区-病房-床位查询条件")
public class WardTreeQuery extends PageQuery {
    @Schema(description = "唯一标识", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "名称", example = "内科疗区", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "机构名称", example = "住院内科")
    private String orgName;

    @Schema(description = "子节点列表")
    private List<WardTreeQuery> children;
}
