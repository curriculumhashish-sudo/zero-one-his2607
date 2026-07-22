package com.zeroone.star.project.query.j4.frequency;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：频次列表条件分页查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "频次分页查询条件对象")
@Getter
@Setter
@ToString(callSuper = true)
public class FrequencyPageQuery extends PageQuery {

    @Schema(description = "模糊查询关键字（支持频次名称或频次代号）", example = "每日")
    private String keyword;
}