package com.zeroone.star.sysmgr.controller.dept;

import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j1.login.DeptSelectDTO;
import com.zeroone.star.project.query.j1.login.DeptQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.login.DepartmentVO;
import com.zeroone.star.project.vo.j1.login.DeptBindResultVO;
import com.zeroone.star.sysmgr.service.dept.DeptService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@Slf4j
@RestController
@RequestMapping("/auth/dept")
@Tag(name = "科室选择接口", description = "医生登录后科室选择相关接口")
public class DeptController {

    @Resource
    private DeptService deptService;

    @Resource
    private UserHolder userHolder;

    // 科室列表查询接口
    @GetMapping("/list")
    @Operation(summary = "获取科室列表", description = "根据医院ID查询可选科室")
    public JsonVO<List<DepartmentVO>> listDept(@Valid DeptQuery query) {
        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }
        return JsonVO.success(deptService.deptList(query));
    }

    // 科室绑定接口
    @PostMapping("/select")
    @Operation(summary = "提交科室选择", description = "绑定用户当前科室")
    public JsonVO<DeptBindResultVO> selectDept(@Valid @RequestBody DeptSelectDTO dto) {
        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }
        return JsonVO.success(deptService.selectDept(dto));
    }

    /**
     * 从请求上下文获取当前登录用户ID，未登录返回null
     */
    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user != null ? user.getId() : null;
        } catch (Exception e) {
            log.warn("获取当前用户失败: {}", e.getMessage());
            return null;
        }
    }
}
