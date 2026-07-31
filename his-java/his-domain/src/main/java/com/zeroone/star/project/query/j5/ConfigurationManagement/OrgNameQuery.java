package com.zeroone.star.project.query.j5.ConfigurationManagement;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：机构名称查询
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@Schema(description = "机构名称查询")
public class OrgNameQuery {

    @NotBlank(message = "机构ID不能为空")
    @Schema(description = "机构ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1816800000000000001")
    private String orgId;
}
