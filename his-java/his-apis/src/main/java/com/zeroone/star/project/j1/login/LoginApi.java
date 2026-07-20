package com.zeroone.star.project.j1.login;

import com.zeroone.star.project.dto.j1.login.SelectHospitalDTO;
import com.zeroone.star.project.dto.j1.login.SelectTenantDTO;
import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.SelectResultVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * j1组 - 登录系统租户/医院选择接口
 */
@Tag(name = "j1-登录-租户医院选择", description = "登录流程中的租户和医院选择相关接口")
public interface LoginApi {

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