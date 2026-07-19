package com.zeroone.star.project.vo.j1.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(description = "用户列表信息")
public class UserSimpleVO {
    @Schema(description = "主键ID", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "用户名", example = "lianglaoshi", requiredMode = Schema.RequiredMode.REQUIRED)
    private String username;

    @Schema(description = "姓名", example = "梁老师", requiredMode = Schema.RequiredMode.REQUIRED)
    private String realName;

    @Schema(description = "手机号码", example = "13800138000")
    private String phone;

    @Schema(description = "电子邮箱", example = "lianglaoshi@hospital.com")
    private String email;

    @Schema(description = "用户状态 (1:启用, 0:停用)", example = "1")
    private Integer status;
}
