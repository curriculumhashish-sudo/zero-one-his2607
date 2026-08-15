package com.zeroone.star.project.query.j2.sysmgr.rent2;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：租户管理查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */
@Data
@Schema(description = "租户管理查询对象")
public class RentQuery extends PageQuery {

    @Schema(description = "租户编码", example = "DEFAULT")
    private String code;

    @Schema(description = "关键词：匹配租户名称/描述", example = "默认")
    private String rentKeywords;

    @Schema(description = "租户状态：1启用 0禁用", example = "1")
    private Integer isActive;
}
