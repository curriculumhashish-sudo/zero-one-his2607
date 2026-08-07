package com.zeroone.star.project.query.j5.ConfigurationManagement;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：执行项目列表查询（映射adm_healthcare_service表）
 * </p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "执行项目列表查询")
public class RunProListQuery extends PageQuery {

    @Schema(description = "项目名称")
    private String name;

    @Schema(description = "类型编码")
    private String typeCode;

    @Schema(description = "分类编码")
    private String categoryCode;

    @Schema(description = "执行地点ID")
    private String locationId;

    @Schema(description = "状态枚举")
    private String statusEnum;

    @Schema(description = "启用标志")
    private Integer activeFlag;
}
