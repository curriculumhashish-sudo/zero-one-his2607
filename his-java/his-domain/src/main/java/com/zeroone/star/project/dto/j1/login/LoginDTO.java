package com.zeroone.star.project.dto.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：用户账号登录请求数据传输对象（DTO）
 * </p>
 * <p>
 * 说明：本类用于封装前端提交的账号登录请求参数，属于<strong>操作请求型DTO</strong>。
 * 请求从终端层(Browser)传入Controller，再透传至Service层进行身份校验。
 * 字段校验规则基于 Jakarta Validation（JSR-380）声明，Controller 方法参数加
 * {@code @Validated} 后生效，校验失败由全局异常处理器返回 9995（PARAMS_INVALID）。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author J1组
 * @version 1.0.0
 */
/*接收前端提交的登录数据，并在进入业务逻辑之前完成参数封装和参数校验。(请求DTO)*/
@Schema(description = "账号登录请求数据传输对象")//描述当前类或字段的信息，供 Swagger/Knife4j 生成接口文档
@Data//自动生成 getter 和 setter 方法
@AllArgsConstructor//自动生成包含所有字段的构造方法
@NoArgsConstructor//自动生成无参构造方法
public class LoginDTO {

    /**
     * 用户名（账户名）
     * <p>对应数据库字段：whale_users.user_name（VARCHAR(256)），字段名与数据库列名保持一致的驼峰形式，
     * 便于与数据持久层实体通过反射自动转换</p>
     * <p>用途：用户登录的身份标识，用于在数据库中查找对应的用户记录</p>
     */
    @NotBlank(message = "用户名不能为空")
    @Size(max = 8, message = "用户名长度不能超过8个字符")
    @Schema(description = "用户名", example = "admin", requiredMode = Schema.RequiredMode.REQUIRED)
    private String userName;

    /**
     * 密码（明文，服务端校验时与密文比对）
     * <p>对应数据库字段：whale_users.password_hash（VARCHAR(256)，存储 BCrypt 加密后的密文），
     * 本字段为前端提交的明文密码，不落库，故不命名为 passwordHash</p>
     * <p>用途：用户身份校验凭证，前端应采用加密方式传输，服务端与数据库中存储的密文进行比对</p>
     * <p>说明：登录是已有密码的校验场景，不做最小长度限制，避免误拒历史弱密码</p>
     */
    @NotBlank(message = "密码不能为空")
    @Size(max = 8, message = "密码长度不能超过8个字符")
    @Schema(description = "密码", example = "123456@@", requiredMode = Schema.RequiredMode.REQUIRED)
    private String password;

    /**
     * 验证码
     * <p>对应数据库字段：无（非持久化字段，属于业务临时数据）</p>
     * <p>用途：防止暴力破解和自动化攻击，由服务端生成图片/短信验证码，登录时校验一致性</p>
     * <p>说明：可选字段，不传则不校验；传入时长度不能超过8位</p>
     */
    @Size(max = 8, message = "验证码长度不能超过4个字符")
    @Schema(description = "验证码", example = "9998")
    private String code;
}
