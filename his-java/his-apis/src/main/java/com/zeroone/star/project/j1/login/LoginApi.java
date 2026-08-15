package com.zeroone.star.project.j1.login;

import com.zeroone.star.project.dto.j1.login.SelectHospitalDTO;
import com.zeroone.star.project.dto.j1.login.SelectTenantDTO;
import com.zeroone.star.project.dto.login.LoginDTO;
import com.zeroone.star.project.dto.login.Oauth2TokenDTO;
import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.LoginVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * j1组 - 登录系统租户/医院选择接口
 */
@Tag(name = "j1-登录-租户医院选择", description = "登录流程中的租户和医院选择相关接口")
public interface LoginApi {

    //用于描述接口的基本信息
    @Operation(
            summary = "账号登录",
            description = "通过用户名、密码和验证码进行身份认证，认证成功后返回访问令牌。"
    )
    @PostMapping("login")
    JsonVO<Oauth2TokenDTO> authLogin(
            @Parameter(description = "登录请求参数")
            LoginDTO loginDTO
    );

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
            description = "注销当前登录用户，使当前访问令牌失效。"
    )
    @PostMapping("logout")
    JsonVO<String> logout();

    /**
     * 当前用户信息
     * <p>接口类型：查询型请求</p>
     * <p>请求参数：无（用户信息从请求头或Token中解析）</p>
     * <p>响应数据：{@link LoginVO}（账户、状态、会话选择及医护人员信息）</p>
     * <p>对应数据库：whale_users + whale_user_login_session + adm_practitioner</p>
     * @return 当前登录用户的详细信息
     */
    @Operation(
            summary = "获取当前用户信息",
            description = "获取当前登录用户的基础信息及角色信息。"
    )
    @GetMapping("/user/info")
    JsonVO<LoginVO> getCurrUser();
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
    JsonVO<List<TenantVO>> getTenantList();

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
    JsonVO<TenantVO> selectTenant(
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
    JsonVO<HospitalVO> selectHospital(
            @Parameter(description = "选择医院请求", required = true) @RequestBody SelectHospitalDTO dto
    );
}
