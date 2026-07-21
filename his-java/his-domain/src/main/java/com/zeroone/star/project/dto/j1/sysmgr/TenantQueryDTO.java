package com.zeroone.star.project.dto.j1.sysmgr;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import lombok.Data;

@Data
@Schema(description = "租户管理查询对象")
public class TenantQueryDTO extends PageQuery {

    @Schema(description = "关键词(用户名或姓名)", example = "梁老师", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String keyword;

    @Schema(description = "用户状态 (1:启用, 0:停用)", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer status;


}
