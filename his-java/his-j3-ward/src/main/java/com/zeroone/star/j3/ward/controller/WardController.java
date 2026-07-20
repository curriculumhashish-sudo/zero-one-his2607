package com.zeroone.star.j3.ward.controller;

import com.zeroone.star.project.dto.j3.ward.*;
import com.zeroone.star.project.j3.ward.WardApis;
import com.zeroone.star.project.query.j3.ward.BedQuery;
import com.zeroone.star.project.query.j3.ward.RoomQuery;
import com.zeroone.star.project.query.j3.ward.WardQuery;
import com.zeroone.star.project.query.j3.ward.WardTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：病区病房管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhu
 * @version 1.0.0
 */
@RestController
@RequestMapping("/ward")
@Tag(name = "病区病房管理", description = "病区-病房-床位相关操作接口")
public class WardController implements WardApis {

    // ==================== 树形结构 ====================

    @Override
    @GetMapping("/tree")
    @Operation(summary = "获取病区-病房-床位树形列表", description = "根据条件机构名称获取树形结构数据")
    public JsonVO<List<WardTreeQuery>> getWardTree(@RequestParam("orgName") String orgName) {
        return null;
    }

    // ==================== 病区管理 ====================

    @Override
    @GetMapping("/list")
    @Operation(summary = "获取病区列表", description = "获取所有病区名称列表（用于下拉框）")
    public JsonVO<List<WardQuery>> getWardList() {
        return null;
    }

    @Override
    @PostMapping
    @Operation(summary = "添加病区", description = "新增一条病区记录")
    public JsonVO<String> addWard(@Valid @RequestBody WardAddDTO wardAddDTO) {
        return null;
    }

    @Override
    @PutMapping("/{id}")
    @Operation(summary = "修改病区", description = "根据ID修改病区信息")
    public JsonVO<String> updateWard(
            @PathVariable("id") Long id,
            @Valid @RequestBody WardDTO wardDTO) {
        return null;
    }

    @Override
    @DeleteMapping("/{id}")
    @Operation(summary = "删除病区", description = "根据ID删除病区记录")
    public JsonVO<String> deleteWard(@PathVariable("id") Long id) {
        return null;
    }

    // ==================== 病房管理 ====================

    @Override
    @GetMapping("/room/list")
    @Operation(summary = "根据病区获取病房列表", description = "根据病区获取病房名称列表")
    public JsonVO<List<RoomQuery>> getRoomListByWard(@RequestParam("wardId") Long wardId) {
        return null;
    }

    @Override
    @PostMapping("/room")
    @Operation(summary = "添加病房", description = "新增一条病房记录")
    public JsonVO<String> addRoom(@Valid @RequestBody RoomAddDTO roomAddDTO) {
        return null;
    }

    @Override
    @PutMapping("/room/{id}")
    @Operation(summary = "修改病房", description = "根据ID修改病房信息")
    public JsonVO<String> updateRoom(
            @PathVariable("id") Long id,
            @Valid @RequestBody RoomDTO roomDTO) {
        return null;
    }

    @Override
    @DeleteMapping("/room/{id}")
    @Operation(summary = "删除病房", description = "根据ID删除病房记录")
    public JsonVO<String> deleteRoom(@PathVariable("id") Long id) {
        return null;
    }

    // ==================== 床位管理 ====================

    @Override
    @GetMapping("/bed/list")
    @Operation(summary = "根据病房获取病床列表", description = "根据病房获取病床名称，包含名称、唯一标识、状态")
    public JsonVO<List<BedQuery>> getBedListByRoom(@RequestParam("roomId") Long roomId) {
        return null;
    }

    @Override
    @PostMapping("/bed")
    @Operation(summary = "添加床位", description = "新增一条床位记录")
    public JsonVO<String> addBed(@Valid @RequestBody BedAddDTO bedAddDTO) {
        return null;
    }

    @Override
    @PutMapping("/bed/{id}")
    @Operation(summary = "修改床位", description = "根据ID修改床位信息")
    public JsonVO<String> updateBed(
            @PathVariable("id") Long id,
            @Valid @RequestBody BedDTO bedDTO) {
        return null;
    }

    @Override
    @DeleteMapping("/bed/{id}")
    @Operation(summary = "删除床位", description = "根据ID删除床位记录")
    public JsonVO<String> deleteBed(@PathVariable("id") Long id) {
        return null;
    }
}
