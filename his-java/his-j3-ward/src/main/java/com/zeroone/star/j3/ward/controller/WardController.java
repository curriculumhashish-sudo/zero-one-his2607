package com.zeroone.star.j3.ward.controller;

import com.zeroone.star.project.dto.j3.ward.*;
import com.zeroone.star.project.j3.ward.WardApis;
import com.zeroone.star.project.query.j3.ward.BedQuery;
import com.zeroone.star.project.query.j3.ward.RoomQuery;
import com.zeroone.star.project.query.j3.ward.WardQuery;
import com.zeroone.star.project.query.j3.ward.WardTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
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
@RequestMapping("/wards")
@Tag(name = "病区病房管理", description = "病区-病房-床位相关操作接口")
public class WardController implements WardApis {

    // ==================== 树形结构 ====================

    @GetMapping("/tree")
    @Operation(summary = "获取病区-病房-床位树形列表", description = "根据条件机构名称获取树形结构数据")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "数据不存在")
    })
    @Override
    public JsonVO<List<WardTreeQuery>> getWardTree(WardTreeQuery query) {
        return null;
    }

    // ==================== 病区管理 ====================

    @GetMapping("/list")
    @Operation(summary = "获取病区列表", description = "获取所有病区名称列表（用于下拉框）")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    })
    @Override
    public JsonVO<List<WardQuery>> getWardList() {
        return null;
    }

    @PostMapping
    @Operation(summary = "添加病区", description = "新增一条病区记录")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "新增成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误")
    })
    @Override
    public JsonVO<String> addWard(@Valid @RequestBody WardAddDTO wardAddDTO) {
        return null;
    }

    @PutMapping("/{id}")
    @Operation(summary = "修改病区", description = "根据ID修改病区信息")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "更新成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "病区不存在")
    })
    @Override
    public JsonVO<String> updateWard(
            @PathVariable("id") Long id,
            @Valid @RequestBody WardDTO wardDTO) {
        return null;
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "删除病区", description = "根据ID删除病区记录")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "删除成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "病区不存在")
    })
    @Override
    public JsonVO<String> deleteWard(@PathVariable("id") Long id) {
        return null;
    }

    // ==================== 病房管理 ====================

    @GetMapping("/room/list")
    @Operation(summary = "根据病区获取病房列表", description = "根据病区获取病房名称列表")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误")
    })
    @Override
    public JsonVO<List<RoomQuery>> getRoomListByWard(@RequestParam("wardId") Long wardId) {
        return null;
    }

    @PostMapping("/room")
    @Operation(summary = "添加病房", description = "新增一条病房记录")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "新增成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误")
    })
    @Override
    public JsonVO<String> addRoom(@Valid @RequestBody RoomAddDTO roomAddDTO) {
        return null;
    }

    @PutMapping("/room/{id}")
    @Operation(summary = "修改病房", description = "根据ID修改病房信息")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "更新成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "病房不存在")
    })
    @Override
    public JsonVO<String> updateRoom(
            @PathVariable("id") Long id,
            @Valid @RequestBody RoomDTO roomDTO) {
        return null;
    }

    @DeleteMapping("/room/{id}")
    @Operation(summary = "删除病房", description = "根据ID删除病房记录")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "删除成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "病房不存在")
    })
    @Override
    public JsonVO<String> deleteRoom(@PathVariable("id") Long id) {
        return null;
    }

    // ==================== 床位管理 ====================

    @GetMapping("/bed/list")
    @Operation(summary = "根据病房获取病床列表", description = "根据病房获取病床名称，包含名称、唯一标识、状态")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误")
    })
    @Override
    public JsonVO<List<BedQuery>> getBedListByRoom(@RequestParam("roomId") Long roomId) {
        return null;
    }

    @PostMapping("/bed")
    @Operation(summary = "添加床位", description = "新增一条床位记录")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "新增成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误")
    })
    @Override
    public JsonVO<String> addBed(@Valid @RequestBody BedAddDTO bedAddDTO) {
        return null;
    }

    @PutMapping("/bed/{id}")
    @Operation(summary = "修改床位", description = "根据ID修改床位信息")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "更新成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "床位不存在")
    })
    @Override
    public JsonVO<String> updateBed(
            @PathVariable("id") Long id,
            @Valid @RequestBody BedDTO bedDTO) {
        return null;
    }

    @DeleteMapping("/bed/{id}")
    @Operation(summary = "删除床位", description = "根据ID删除床位记录")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "删除成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "床位不存在")
    })
    @Override
    public JsonVO<String> deleteBed(@PathVariable("id") Long id) {
        return null;
    }
}
