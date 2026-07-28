package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;


@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "用户科室绑定结果")
public class DeptBindResultVO {

    @Schema(description = "选中科室id集合")
    private String organizationUnitId;
}
