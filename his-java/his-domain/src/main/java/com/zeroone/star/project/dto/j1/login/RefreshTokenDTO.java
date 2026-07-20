package com.zeroone.star.project.dto.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：凭证刷新请求数据传输对象（DTO）
 * </p>
 * <p>
 * 说明：本类用于封装前端在访问令牌过期后提交的刷新请求参数，属于<strong>操作请求型DTO</strong>。
 * 请求从终端层传入 Controller，携带旧的 Token 和 RefreshToken，换取新的 Token 对。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author J1组
 * @version 1.0.0
 */
/*当前 Token 快过期时，前端提交给后端的数据。（请求DTO）*/
@Schema(description = "凭证刷新请求数据传输对象")
@Data
public class RefreshTokenDTO {

    /**
     * 访问令牌（当前即将过期的 Access Token）
     * <p>对应数据库字段：无（Token 属于 OAuth2 运行时数据，不持久化到数据库）</p>
     * <p>用途：携带当前访问令牌，服务端解析其中的会话信息（如 jti、用户 ID 等），
     * 用于关联到对应的刷新令牌记录</p>
     */
    @Schema(description = "当前访问令牌", requiredMode = Schema.RequiredMode.REQUIRED,
            example = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9...")
    private String token;

    /**
     * 刷新令牌（Refresh Token）
     * <p>对应数据库字段：无（Token 属于 OAuth2 运行时数据，不持久化到数据库）</p>
     * <p>用途：携带刷新令牌，服务端验证其有效性后，生成新的 Access Token 和 Refresh Token 返回给前端</p>
     */
    @Schema(description = "刷新令牌", requiredMode = Schema.RequiredMode.REQUIRED,
            example = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9...")
    private String refreshToken;
}
