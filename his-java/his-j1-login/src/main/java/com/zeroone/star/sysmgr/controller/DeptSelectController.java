package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.j1.login.DeptQueryDTO;
import com.zeroone.star.project.dto.j1.login.DeptSelectDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.login.DepartmentVO;
import com.zeroone.star.project.vo.j1.login.DeptBindResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/auth/dept")
@Tag(name = "科室选择接口", description = "医生登录后科室选择相关接口")
public class DeptSelectController {

    // 科室列表查询接口
    @GetMapping("/list")
    @Operation(summary = "获取科室列表", description = "根据医院ID查询可选科室")
    public JsonVO<List<DepartmentVO>> listDepts(@Valid DeptQueryDTO dto) {
        // 后续实现业务逻辑
        return JsonVO.success(null);
    }

    // 科室绑定接口
    @PostMapping("/select")
    @Operation(summary = "提交科室选择", description = "绑定用户当前科室")
    public JsonVO<DeptBindResultVO> selectDept(@Valid @RequestBody DeptSelectDTO dto) {
        // 后续实现业务逻辑
        return JsonVO.success(null);
    }
}
