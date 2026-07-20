package com.zeroone.star.project.dto.j1.login;

import com.fasterxml.jackson.annotation.JsonIgnore;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：OAuth2 授权登录响应数据传输对象（DTO）
 * </p>
 * <p>
 * 说明：本类用于封装 OAuth2 认证成功后的 Token 响应数据。
 * 按四层架构规范，响应数据应使用 VO，但因历史接口已约定返回该 DTO，
 * 故保持兼容性子啊在 DTO 包中定义。实际属于<strong>视图展示数据</strong>。
 * 数据由 Service 层组装后返回给 Controller，最终包装成 JsonVO 响应给前端。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author J1组
 * @version 1.0.0
 */
/*封装登录成功后返回给前端的 Token 信息。(响应DTO)*/
@Schema(description = "OAuth2 授权登录响应数据传输对象")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Oauth2TokenDTO {

    /**
     * 访问令牌（Access Token）
     * <p>对应数据库字段：无（Token 属于 OAuth2 运行时生成的 JWT 数据，不持久化到数据库）</p>
     * <p>用途：JWT 格式的访问令牌，包含用户 ID、用户名、角色权限、过期时间等声明，
     * 前端在后续请求中将其放入 HTTP Authorization 头部进行身份认证</p>
     */
    @Schema(description = "授权访问令牌（JWT）", requiredMode = Schema.RequiredMode.REQUIRED,
            example = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9...")
    private String token;

    /**
     * 刷新令牌（Refresh Token）
     * <p>对应数据库字段：无（Token 属于 OAuth2 运行时生成的 JWT 数据，不持久化到数据库）</p>
     * <p>用途：当访问令牌（Access Token）过期后，前端使用刷新令牌向服务端申请新的访问令牌，
     * 无需重新输入用户名和密码，提升用户体验</p>
     */
    @Schema(description = "刷新令牌", requiredMode = Schema.RequiredMode.REQUIRED,
            example = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9...")
    private String refreshToken;

    /**
     * 访问令牌头前缀
     * <p>对应数据库字段：无</p>
     * <p>用途：HTTP Authorization 请求头的令牌类型前缀，标准值为 "Bearer "，
     * 前端拼接格式为：Authorization: Bearer &lt;token&gt;</p>
     */
    @Schema(description = "访问令牌前缀", requiredMode = Schema.RequiredMode.REQUIRED, example = "Bearer ")
    private String tokenHead;

    /**
     * 有效时间（秒）
     * <p>对应数据库字段：无</p>
     * <p>用途：访问令牌的有效时长（单位：秒），服务端内部使用，标记为 {@link JsonIgnore} 避免暴露给前端</p>
     */
    @JsonIgnore
    @Schema(description = "有效时间（秒）", example = "3599", hidden = true)
    private Integer expiresIn;
}
