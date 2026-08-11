package com.zeroone.star.sysmgr.controller.tenant;

import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.constant.RedisConstant;
import com.zeroone.star.project.dto.j1.login.SelectHospitalDTO;
import com.zeroone.star.project.dto.j1.login.SelectTenantDTO;
import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sysmgr.service.tenant.TenantService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.ConstraintViolation;
import jakarta.validation.Validator;
import lombok.extern.slf4j.Slf4j;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Comparator;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

@Slf4j
@Tag(name = "租户医院选择")
@RestController
@RequestMapping("/auth")
public class TenantController {

    @Resource
    private UserHolder userHolder;

    @Resource
    private RedisTemplate<String, Object> redisTemplate;

    @Resource
    private Validator validator;

    @Resource
    private TenantService tenantService;

    // ==================== 接口 1：获取可选租户列表 ====================

    @Operation(summary = "获取可选租户列表")
    @GetMapping("tenants")
    public JsonVO<List<TenantVO>> getTenantList() {
        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }

        List<TenantVO> list = tenantService.getTenantList(userId);
        return JsonVO.success(list);
    }

    // ==================== 接口 2：获取可选医院列表 ====================

    @Operation(summary = "获取可选医院列表")
    @GetMapping("hospitals")
    public JsonVO<List<HospitalVO>> getHospitalList(HospitalQuery query) {
        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }
        String currentTenantId = getCurrentTenantId(userId);
        if (currentTenantId == null) {
            return JsonVO.fail("请先选择租户");
        }

        List<HospitalVO> list = tenantService.getHospitalList(currentTenantId, query);
        return JsonVO.success(list);
    }

    // ==================== 接口 3：选择租户 ====================

    @Operation(summary = "选择租户")
    @PostMapping("select")
    public JsonVO<TenantVO> selectTenant(@RequestBody SelectTenantDTO dto) {
        String validErr = validate(dto);
        if (validErr != null) {
            return JsonVO.fail(validErr);
        }

        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }

        boolean hasPermission = tenantService.checkUserTenant(userId, dto.getTenantId());
        if (!hasPermission) {
            return JsonVO.fail("无权访问该租户");
        }

        tenantService.cacheSelectedTenant(userId, dto.getTenantId());

        TenantVO tenant = tenantService.getTenantById(dto.getTenantId());
        if (tenant == null) {
            return JsonVO.fail("租户不存在或已停用");
        }
        return JsonVO.success(tenant);
    }

    // ==================== 接口 4：选择医院 ====================

    @Operation(summary = "选择医院")
    @PostMapping("hospital/select")
    public JsonVO<HospitalVO> selectHospital(@RequestBody SelectHospitalDTO dto) {
        String validErr = validate(dto);
        if (validErr != null) {
            return JsonVO.fail(validErr);
        }

        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }
        String currentTenantId = getCurrentTenantId(userId);
        if (currentTenantId == null) {
            return JsonVO.fail("请先选择租户");
        }

        boolean valid = tenantService.checkHospitalBelongsToTenant(dto.getHospitalId(), currentTenantId);
        if (!valid) {
            return JsonVO.fail("该医院不在当前租户下或已停用");
        }

        tenantService.cacheSelectedHospital(userId, dto.getHospitalId());

        HospitalVO hospital = tenantService.getHospitalById(dto.getHospitalId());
        if (hospital == null) {
            return JsonVO.fail("医院不存在或已停用");
        }
        return JsonVO.success(hospital);
    }

    // ==================== 私有工具方法 ====================

    private <T> String validate(T obj) {
        if (obj == null) {
            return "请求参数不能为空";
        }
        Set<ConstraintViolation<T>> violations = validator.validate(obj);
        if (!violations.isEmpty()) {
            return violations.stream()
                    .sorted(Comparator.comparing(v -> v.getPropertyPath().toString()))
                    .map(v -> "[" + v.getPropertyPath() + "]" + v.getMessage())
                    .collect(Collectors.joining("; "));
        }
        return null;
    }

    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user != null ? user.getId() : null;
        } catch (Exception e) {
            log.warn("获取当前用户失败: {}", e.getMessage());
            return null;
        }
    }

    /**
     * 从 Redis user:session:{userId} 取当前已选租户
     */
    private String getCurrentTenantId(String userId) {
        String key = RedisConstant.USER_SESSION_PREFIX + userId;
        Object tenantId = redisTemplate.opsForHash().get(key, "tenant_id");
        if (tenantId == null) {
            return null;
        }
        return tenantId.toString();
    }
}