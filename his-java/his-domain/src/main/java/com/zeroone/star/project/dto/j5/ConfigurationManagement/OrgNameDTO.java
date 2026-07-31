package com.zeroone.star.project.dto.j5.ConfigurationManagement;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：机构名称DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@Schema(description = "机构名称DTO")
public class OrgNameDTO {

    @Schema(description = "机构ID", example = "1816800000000000001")
    private String orgId;

    @Schema(description = "机构名称", example = "门诊部")
    private String orgName;
}
