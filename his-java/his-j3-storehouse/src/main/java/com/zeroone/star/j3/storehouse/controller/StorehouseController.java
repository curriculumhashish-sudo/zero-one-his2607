package com.zeroone.star.j3.storehouse.controller;

import com.zeroone.star.j3.storehouse.service.StoreHouseService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseAddDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseUpdateDTO;
import com.zeroone.star.project.j3.storehouse.StoreHouseApis;
import com.zeroone.star.project.query.j3.storehouse.StoreHouseQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.storehouse.DepartmentSimpleVO;
import com.zeroone.star.project.vo.j3.storehouse.StoreHouseVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 库房药房管理控制器
 * @author mie
 * @version 1.0.0
 */
@RestController
@RequestMapping("/storehouses")
@Slf4j
@Tag(name = "库房药房管理", description = "库房药房相关操作接口")
public class StorehouseController implements StoreHouseApis {

    @Resource
    private StoreHouseService storeHouseService;

    @Override
    @GetMapping("/getStoreHouseNames")
    @Operation(summary = "获取库房药房名称", description = "支持按形态类型筛选，传null返回全部")
    public JsonVO<List<StoreHouseVO>> getStoreHouseNames(@RequestParam(required = false) String formEnum) {
        List<StoreHouseVO> list = storeHouseService.getStoreHouseNames(formEnum);
        return JsonVO.success(list);
    }

    @Override
    @GetMapping("/list")
    @Operation(summary = "获取库房药房列表（条件+分页）", description = "支持按名称/拼音码模糊搜索、形态筛选、状态筛选，返回分页结果")
    public JsonVO<PageDTO<StoreHouseVO>> list(@Valid StoreHouseQuery query) {
        PageDTO<StoreHouseVO> page = storeHouseService.list(query);
        return JsonVO.success(page);
    }

    @Override
    @PostMapping("/add")
    @Operation(summary = "添加库房药房", description = "新增一条库房药房记录")
    public JsonVO<String> add(@Valid @RequestBody StoreHouseAddDTO dto) {
        storeHouseService.add(dto);
        return JsonVO.success("添加成功");
    }

    @Override
    @PutMapping("/update")
    @Operation(summary = "修改库房药房", description = "根据ID修改库房药房信息")
    public JsonVO<String> update(@Valid @RequestBody StoreHouseUpdateDTO dto) {
        storeHouseService.update(dto);
        return JsonVO.success("修改成功");
    }

    @Override
    @DeleteMapping("/delete/{id}")
    @Operation(summary = "删除库房药房", description = "根据ID删除库房药房记录")
    public JsonVO<String> delete(@PathVariable Long id) {
        storeHouseService.delete(id);
        return JsonVO.success("删除成功");
    }

    @Override
    @PutMapping("/toggleEnabled")
    @Operation(summary = "启/停用库房药房", description = "切换库房药房的启用状态")
    public JsonVO<String> toggleEnabled(@RequestParam Long id, @RequestParam Byte isEnabled) {
        storeHouseService.toggleEnabled(id, isEnabled);
        String statusText = isEnabled == 1 ? "启用" : "停用";
        return JsonVO.success(statusText + "成功");
    }

    @Override
    @GetMapping("/getparentDepartments")
    @Operation(summary = "获取上级科室列表", description = "用于科室药房选择上级科室")
    public JsonVO<List<DepartmentSimpleVO>> getParentDepartments() {
        List<DepartmentSimpleVO> list = storeHouseService.listParentDepartments();
        return JsonVO.success(list);
    }
}
