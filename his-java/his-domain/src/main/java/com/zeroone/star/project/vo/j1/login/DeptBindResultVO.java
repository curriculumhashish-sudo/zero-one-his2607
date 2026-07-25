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

    @Schema(description = "是否成功 true成功 false失败")
    private Boolean success;

    @Schema(description = "提示信息")
    private String msg;

    @Schema(description = "本次绑定科室数量")
    private Integer bindCount;
}
