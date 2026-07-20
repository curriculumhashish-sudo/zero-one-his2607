package com.zeroone.star.project.vo.j1.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "选择用户列表对象")
public class UserListVO {
    @Schema(description = "主键id", example = "336396155648217088")
    private String id;

    @Schema(description = "用户名", example = "lianglaoshi")
    private String username;

    @Schema(description = "姓名", example = "梁老师")
    private String nickname;

    @Schema(description = "电子邮箱", example = "lianglaoshi@hospital.com")
    private String email;

    @Schema(description = "手机号码", example = "13800138000")
    private String phoneNumber;

    @Schema(description = "用户状态 (1:启用, 0:停用)", example = "1")
    private Integer status;
}
