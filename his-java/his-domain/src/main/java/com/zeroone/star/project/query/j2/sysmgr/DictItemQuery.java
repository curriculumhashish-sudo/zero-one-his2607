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
@Schema(description = "字典编码查询对象")
public class DictItemQuery extends PageQuery {
    @Schema(description = "字典类型编码", example = "ADMISSION", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String dictCode;
    @Schema(description = "关键词:可以是字典列表名称或代码", example = "入院", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String dicitemKeyword;
}
