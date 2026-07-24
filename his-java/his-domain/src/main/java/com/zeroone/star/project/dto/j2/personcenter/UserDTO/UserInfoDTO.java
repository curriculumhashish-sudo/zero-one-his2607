package com.zeroone.star.project.dto.j2.personcenter.UserDTO;
import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.fasterxml.jackson.databind.ser.std.ToStringSerializer;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.experimental.Accessors;

@Data
@Accessors(chain = true)
@Schema(description = "用户信息详情")
public class UserInfoDTO {

    @Schema(description = "用户ID", example = "1234567890123456789")
    @JsonSerialize(using = ToStringSerializer.class)
    private Long id;

    @Schema(description = "用户名", example = "admin")
    private String userName;

    @Schema(description = "真实姓名", example = "张三")
    private String realName;

    @Schema(description = "头像", example = "https://example.com/avatar.jpg")
    private String avatar;

    @Schema(description = "邮箱", example = "zhangsan@example.com")
    private String email;

    @Schema(description = "手机号", example = "13800138000")
    private String phoneNumber;

    @Schema(description = "用户状态：true-启用 false-停用", example = "true")
    private Boolean isActive;

    @Schema(description = "是否锁定", example = "false")
    private Boolean isLockedOut;

    @Schema(description = "租户ID", example = "1")
    @JsonSerialize(using = ToStringSerializer.class)
    private Long tenantId;

    @Schema(description = "所属机构ID（可选）", example = "10001")
    @JsonSerialize(using = ToStringSerializer.class)
    private Long organizationId;

    @Schema(description = "所属执业人员ID（可选）", example = "20001")
    @JsonSerialize(using = ToStringSerializer.class)
    private Long practitionerId;

    @Schema(description = "创建时间", example = "2026-01-01T10:00:00")
    private String createdAt;

    @Schema(description = "更新时间", example = "2026-01-01T10:00:00")
    private String updatedAt;
}