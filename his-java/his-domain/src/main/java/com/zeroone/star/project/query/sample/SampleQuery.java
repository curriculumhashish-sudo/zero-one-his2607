package com.zeroone.star.project.query.sample;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：示例查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "示例查询对象")
public class SampleQuery extends PageQuery {
    @Schema(description = "姓名", example = "张三")
    private String name;
}
