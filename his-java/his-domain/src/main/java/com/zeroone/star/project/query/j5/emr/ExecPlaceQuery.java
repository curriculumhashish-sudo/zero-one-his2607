package com.zeroone.star.project.query.j5.emr;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：执行地点查询条件对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "执行地点查询条件对象")
public class ExecPlaceQuery extends PageQuery {

    /**
     * 执行地点编码（模糊查询）
     */
    @Schema(description = "执行地点编码（模糊查询）", example = "EP001")
    private String placeCode;

    /**
     * 执行地点名称（模糊查询）
     */
    @Schema(description = "执行地点名称（模糊查询）", example = "门诊注射室")
    private String placeName;

    /**
     * 是否启用 0-禁用 1-启用
     */
    @Schema(description = "是否启用 0-禁用 1-启用", example = "1")
    private Integer activeFlag;
}
