package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.DeptStockService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.deptstock.DeptStockSaveDTO;
import com.zeroone.star.project.j5.deptstock.DeptStockApis;
import com.zeroone.star.project.query.j5.deptstock.DeptStockQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.deptstock.DeptStockVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * 配置管理-取药科室控制器
 */
@RestController
@RequestMapping("/dept-stock")
@Tag(name = "配置管理-取药科室", description = "取药科室管理接口")
@Validated
@RequiredArgsConstructor
public class DeptStockController implements DeptStockApis {

    private final DeptStockService deptStockService;

    @Override
    @GetMapping("/list")
    @Operation(summary = "获取科室项目列表", description = "条件+分页查询取药科室项目")
    public JsonVO<PageDTO<DeptStockVO>> listDeptStock(DeptStockQuery query) {
        return deptStockService.listDeptStock(query);
    }

    @Override
    @PostMapping("/save")
    @Operation(summary = "保存科室项目", description = "批量保存取药科室项目")
    public JsonVO<String> saveDeptStock(@Valid @RequestBody DeptStockSaveDTO dto) {
        return deptStockService.saveDeptStock(dto);
    }
}
