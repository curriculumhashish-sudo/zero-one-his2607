package com.zeroone.star.project.j1.login;

import com.zeroone.star.project.dto.j1.login.*;
import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.login.*;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;

import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：J1组登录系统接口
 * </p>
 *
 * <p>
 * 说明：
 * 定义登录模块对外接口契约。
 * 不负责业务实现，只负责接口声明。
 *
 * 包含：
 * 1. 用户账号登录
 * 2. Token退出
 * 3. 当前用户信息
 * 4. 租户选择
 * 5. 医院选择
 *
 * </p>
 *
 * @author J1组
 * @version 1.0.0
 */
@Tag(name = "j1-登录-租户医院选择", description = "登录流程中的租户和医院选择相关接口")
public interface LoginApi {
    @Tag(name = "登录认证", description = "J1组登录认证第一组接口：账号登录、退出登录、当前用户信息")
    public interface LoginApis {

        /**
         * 账号登录
         * <p>接口类型：操作型请求</p>
         * <p>请求参数：{@link LoginDTO}（用户名、密码、验证码）</p>
         * <p>响应数据：{@link Oauth2TokenDTO}（访问令牌、刷新令牌、前缀、有效期）</p>
         * <p>对应数据库：whale_users（身份校验）</p>
         * @param loginDTO 登录请求数据对象
         * @return 授权登录结果，包含 OAuth2 访问令牌和刷新令牌
         */
        //用于描述接口的基本信息
        @Operation(
                summary = "账号登录",
                description = "通过用户名、密码和验证码进行身份认证，成功后返回OAuth2访问令牌。"
                        + "用户名对应数据库 whale_users.user_name，密码与 whale_users.password_hash 中的密文比对校验。"
        )
        //用于描述接口可能返回的各种响应结果
        @ApiResponses({
                @ApiResponse(responseCode = "10000", description = "登录成功，返回Token信息"),
                @ApiResponse(responseCode = "9999", description = "登录失败：用户名或密码错误、验证码错误或已过期"),
                @ApiResponse(responseCode = "401", description = "暂未登录或Token已经过期")
        })
        JsonVO<Oauth2TokenDTO> authLogin(LoginDTO loginDTO);

        /**
         * 退出登录
         * <p>接口类型：操作型请求</p>
         * <p>请求参数：无（Token从请求头 Authorization 中解析获取）</p>
         * <p>响应数据：String（退出结果提示信息，如"退出成功"）</p>
         * <p>业务说明：注销当前用户会话，清除服务器端的Token认证状态</p>
         * @return 退出登录结果
         */
        @Operation(
                summary = "退出登录",
                description = "注销当前用户会话，清除服务器端的Token认证状态。"
                        + "从请求头 Authorization 中解析当前Token，调用OAuth2服务撤销该Token使其失效。"
        )
        @ApiResponses({
                @ApiResponse(responseCode = "10000", description = "退出成功"),
                @ApiResponse(responseCode = "9999", description = "退出失败：未获取到Token或Token撤销失败"),
                @ApiResponse(responseCode = "401", description = "暂未登录或Token已经过期")
        })
        JsonVO<String> logout();

        /**
         * 当前用户信息
         * <p>接口类型：查询型请求</p>
         * <p>请求参数：无（用户信息从请求头或Token中解析）</p>
         * <p>响应数据：{@link LoginVO}（用户ID、用户名、启用状态、角色编码列表）</p>
         * <p>对应数据库：whale_users + whale_user_roles + whale_roles（关联查询）</p>
         * @return 当前登录用户的详细信息
         */
        @Operation(
                summary = "当前用户信息",
                description = "获取当前登录用户的详细信息，包括用户ID、用户名、启用状态和角色编码列表。"
                        + "字段与数据库对应：id/userName/isActive 对应 whale_users.id/user_name/is_active，"
                        + "roles 通过 whale_user_roles 关联 whale_roles 查询角色编码 code。"
        )
        @ApiResponses({
                @ApiResponse(responseCode = "10000", description = "获取成功"),
                @ApiResponse(responseCode = "9999", description = "获取失败：用户未登录或Token解析失败"),
                @ApiResponse(responseCode = "401", description = "暂未登录或Token已经过期")
        })
        JsonVO<LoginVO> getCurrUser();
    }

    /**
     * 获取可选租户列表
     * @param query 租户查询对象
     * @return 租户信息列表
     */
    @Operation(
            summary = "获取可选租户列表",
            description = "根据当前登录用户获取其可选的租户列表，如：默认租户、测试租户"
    )
    @GetMapping("tenants")
    JsonVO<List<TenantVO>> getTenantList(
            @Parameter(description = "租户查询条件") TenantQuery query
    );

    /**
     * 选择租户
     * @param dto 租户选择对象
     * @return 选择结果
     */
    @Operation(
            summary = "选择租户",
            description = "服务端记住当前登录用户选择的租户，后续操作基于此租户"
    )
    @PostMapping("select")
    JsonVO<SelectResultVO> selectTenant(
            @Parameter(description = "选择租户请求", required = true) @RequestBody SelectTenantDTO dto
    );

    /**
     * 获取可选医院列表
     * @param query 医院查询对象
     * @return 医院信息列表
     */
    @Operation(
            summary = "获取可选医院列表",
            description = "根据租户信息获取可选医院列表。注意：租户和医院一一绑定，一个租户对应一个医院"
    )
    @GetMapping("hospitals")
    JsonVO<List<HospitalVO>> getHospitalList(
            @Parameter(description = "医院查询条件") HospitalQuery query
    );

    /**
     * 选择医院
     * @param dto 医院选择对象
     * @return 选择结果
     */
    @Operation(
            summary = "选择医院",
            description = "服务端记住当前用户选择的医院。虽然租户和医院一一绑定，为了支持扩展设计此接口"
    )
    @PostMapping("hospital/select")
    JsonVO<SelectResultVO> selectHospital(
            @Parameter(description = "选择医院请求", required = true) @RequestBody SelectHospitalDTO dto
    );
}
