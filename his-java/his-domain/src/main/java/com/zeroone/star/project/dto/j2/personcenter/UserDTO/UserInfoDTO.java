package com.zeroone.star.project.dto.j2.personcenter.UserDTO;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.experimental.Accessors;

@Data
@Accessors(chain = true)
@Schema(description = "用户信息详情")
public class UserInfoDTO {

    @Schema(description = "用户ID", example = "1234567890123456789")
    private String userId;

    @Schema(description = "用户名", example = "admin")
    private String userName;

    @Schema(description = "名", example = "三")
    private String name;

    @Schema(description = "姓", example = "张")
    private String surname;

    @Schema(description = "邮箱", example = "zhangsan@example.com")
    private String email;

    @Schema(description = "手机号", example = "13800138000")
    private String phoneNumber;

    @Schema(description = "用户状态：true-启用 false-停用", example = "true")
    private Boolean isActive;

    @Schema(description = "是否锁定", example = "false")
    private Boolean isLockedOut;

    @Schema(description = "创建时间", example = "2026-01-01T10:00:00")
    private String createdAt;

    @Schema(description = "更新时间", example = "2026-01-01T10:00:00")
    private String updatedAt;
}