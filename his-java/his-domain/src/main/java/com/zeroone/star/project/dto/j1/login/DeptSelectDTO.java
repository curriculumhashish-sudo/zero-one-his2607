package com.zeroone.star.project.dto.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;
import java.util.List;

@Data
@Schema(description = "用户绑定选中科室入参")
public class DeptSelectDTO {

    @Schema(description = "用户id", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotEmpty(message = "用户id不能为空")
    private String userId;

    @Schema(description = "选中科室id集合", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotEmpty(message = "至少选择一个科室")
    private List<String> organizationUnitIdList;
}
