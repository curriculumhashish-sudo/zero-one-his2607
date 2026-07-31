package com.zeroone.star.project.j5.deptstock;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.deptstock.DeptStockSaveDTO;
import com.zeroone.star.project.query.j5.deptstock.DeptStockQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.deptstock.DeptStockVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

/**
 * 配置管理-取药科室API接口
 */
@Tag(name = "配置管理-取药科室", description = "取药科室管理接口")
@RequestMapping("/dept-stock")
public interface DeptStockApis {

    /**
     * 获取科室项目列表（条件+分页）
     */
    @Operation(summary = "获取科室项目列表", description = "条件+分页查询取药科室项目")
    @GetMapping("/list")
    JsonVO<PageDTO<DeptStockVO>> listDeptStock(DeptStockQuery query);

    /**
     * 保存科室项目（批量新增或更新）
     */
    @Operation(summary = "保存科室项目", description = "批量保存科室项目")
    @PostMapping("/save")
    JsonVO<String> saveDeptStock(@Valid @RequestBody DeptStockSaveDTO dto);
}
