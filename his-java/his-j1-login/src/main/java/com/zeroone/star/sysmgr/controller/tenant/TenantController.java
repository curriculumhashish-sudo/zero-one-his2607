package com.zeroone.star.sysmgr.controller.tenant;

import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.constant.RedisConstant;
import com.zeroone.star.project.dto.j1.login.SelectHospitalDTO;
import com.zeroone.star.project.dto.j1.login.SelectTenantDTO;
import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.SelectResultVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@Tag(name = "租户医院选择")
@RestController
@RequestMapping("/auth")
public class TenantController {

    private static final Logger log = LoggerFactory.getLogger(TenantController.class);

    @Resource
    private UserHolder userHolder;

    @Resource
    private RedisTemplate<String, Object> redisTemplate;

    @GetMapping("tenants")
    public JsonVO<List<TenantVO>> getTenantList(TenantQuery query) {
        return JsonVO.success(null);
    }

    @GetMapping("hospitals")
    public JsonVO<List<HospitalVO>> getHospitalList(HospitalQuery query) {
        return JsonVO.success(null);
    }

    @PostMapping("select")
    public JsonVO<SelectResultVO> selectTenant(@RequestBody SelectTenantDTO dto) {
        if (dto == null || dto.getTenantId() == null) {
            return JsonVO.fail("租户ID不能为空");
        }
        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }
        try {
            String key = RedisConstant.USER_SESSION_PREFIX + userId;
            redisTemplate.opsForHash().put(key, "tenant_id", dto.getTenantId().toString());
            SelectResultVO result = new SelectResultVO();
            result.setSuccess(true);
            result.setCurrentTenantId(dto.getTenantId());
            return JsonVO.success(result);
        } catch (Exception e) {
            log.warn("更新 Redis 租户选择失败: {}", e.getMessage());
            return JsonVO.fail("更新租户选择失败");
        }
    }

    @PostMapping("hospital/select")
    public JsonVO<SelectResultVO> selectHospital(@RequestBody SelectHospitalDTO dto) {
        if (dto == null || dto.getHospitalId() == null) {
            return JsonVO.fail("医院ID不能为空");
        }
        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }
        try {
            String key = RedisConstant.USER_SESSION_PREFIX + userId;
            redisTemplate.opsForHash().put(key, "hospital_organization_id", dto.getHospitalId().toString());
            SelectResultVO result = new SelectResultVO();
            result.setSuccess(true);
            result.setCurrentHospitalId(dto.getHospitalId());
            return JsonVO.success(result);
        } catch (Exception e) {
            log.warn("更新 Redis 医院选择失败: {}", e.getMessage());
            return JsonVO.fail("更新医院选择失败");
        }
    }

    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user != null ? user.getId() : null;
        } catch (Exception e) {
            return null;
        }
    }
}
