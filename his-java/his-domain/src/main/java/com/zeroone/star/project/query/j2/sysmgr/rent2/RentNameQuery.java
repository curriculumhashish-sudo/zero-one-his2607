package com.zeroone.star.project.query.j2.sysmgr.rent2;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：租户名称查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */
@Data
@Schema(description = "租户名称查询对象")
public class RentNameQuery {
    @Schema(description = "租户名称、编码模糊搜索关键字", example = "默认")
    private String keyword;
}
