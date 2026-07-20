package com.zeroone.star.project.query.j2.sysmgr;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：租户管理查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "租户管理查询对象")
public class RentQuery extends PageQuery {
    @Schema(description = "租户编码", example = "01", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String rentCode;
    @Schema(description = "关键词:可以是租户名称或描述", example = "默认租户", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String rentKeyword;
    @Schema(description = "租户状态:1 启用 0 禁用", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String rentStatus;

}
