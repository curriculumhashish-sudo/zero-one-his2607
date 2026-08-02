package com.zeroone.star.project.vo.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：登录显示数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Schema(description = "登录显示对象")
@Data
public class LoginVO {

    @Schema(description = "用户唯一编号（前端契约字段，与 id 相同）", example = "1")
    private String userId;

    @Schema(description = "用户唯一编号", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "登录账号", example = "admin", requiredMode = Schema.RequiredMode.REQUIRED)
    private String username;

    @Schema(description = "用户真实姓名（前端契约字段，与 name 相同）", example = "张三")
    private String realName;

    @Schema(description = "用户姓名", example = "张三")
    private String name;

    @Schema(description = "用户头像地址", example = "")
    private String avatar;

    @Schema(description = "用户描述/个性签名", example = "")
    private String desc;

    @Schema(description = "登录后跳转首页路径", example = "/dashboard")
    private String homePath;

    @Schema(description = "访问令牌", example = "eyJhbGciOi...")
    private String token;

    @Schema(description = "邮箱", example = "zhangsan@example.com")
    private String email;

    @Schema(description = "手机号", example = "13800138000")
    private String phoneNumber;

    @Schema(description = "是否启用 1 启用 0 禁用 ", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Byte isEnabled;

    @Schema(description = "是否锁定", example = "false")
    private Boolean lockedOut;

    @Schema(description = "已选择的租户ID")
    private String tenantId;

    @Schema(description = "已选择的医院组织ID")
    private String hospitalOrganizationId;

    @Schema(description = "已选择的组织单元ID")
    private String organizationUnitId;

    @Schema(description = "关联医护人员ID")
    private String practitionerId;

    @Schema(description = "关联医护人员姓名")
    private String practitionerName;

    @Schema(description = "医护人员状态", example = "ACTIVE")
    private String practitionerStatus;
}
