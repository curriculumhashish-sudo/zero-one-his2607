package com.zeroone.star.j3.ward.controller;

import com.zeroone.star.project.dto.j3.ward.BedAddDTO;
import com.zeroone.star.project.dto.j3.ward.BedDTO;
import com.zeroone.star.project.dto.j3.ward.RoomAddDTO;
import com.zeroone.star.project.dto.j3.ward.RoomDTO;
import com.zeroone.star.project.dto.j3.ward.WardAddDTO;
import com.zeroone.star.project.dto.j3.ward.WardDTO;
import com.zeroone.star.project.j3.ward.WardApis;
import com.zeroone.star.project.query.j3.ward.WardTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.ward.BedOptionVO;
import com.zeroone.star.project.vo.j3.ward.OptionVO;
import com.zeroone.star.project.vo.j3.ward.WardTreeVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：病区病房管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@RestController
@RequestMapping("/wards")
@Slf4j
@Tag(name = "病区病房管理")
public class WardController implements WardApis {

    // ==================== 树形结构 ====================

    @PostMapping("/tree")
    @Operation(summary = "获取病区-病房-床位树形列表", description = "根据条件机构名称获取树形结构数据")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "数据不存在")
    })
    @Override
    public JsonVO<List<WardTreeVO>> getWardTree(@RequestBody WardTreeQuery query) {
        return JsonVO.success(null);
    }

    // ==================== 病区管理 ====================

    @GetMapping("/list")
    @Operation(summary = "获取病区列表", description = "获取所有病区名称列表（用于下拉框）")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    })
    @Override
    public JsonVO<List<OptionVO>> getWardList() {
        return JsonVO.success(null);
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
        return JsonVO.success(null);
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
            @Parameter(description = "病区ID", example = "1", required = true)
            @PathVariable("id") Long id,
            @Valid @RequestBody WardDTO wardDTO) {
        return JsonVO.success(null);
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
    public JsonVO<String> deleteWard(
            @Parameter(description = "病区ID", example = "1", required = true)
            @PathVariable("id") Long id) {
        return JsonVO.success(null);
    }

    // ==================== 病房管理 ====================

    @GetMapping("/room/list")
    @Operation(summary = "根据病区获取病房列表", description = "根据病区获取病房名称列表（用于下拉框）")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误")
    })
    @Override
    public JsonVO<List<OptionVO>> getRoomListByWard(
            @Parameter(description = "病区ID", example = "1", required = true)
            @RequestParam("wardId") Long wardId) {
        return JsonVO.success(null);
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
        return JsonVO.success(null);
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
            @Parameter(description = "病房ID", example = "1", required = true)
            @PathVariable("id") Long id,
            @Valid @RequestBody RoomDTO roomDTO) {
        return JsonVO.success(null);
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
    public JsonVO<String> deleteRoom(
            @Parameter(description = "病房ID", example = "1", required = true)
            @PathVariable("id") Long id) {
        return JsonVO.success(null);
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
    public JsonVO<List<BedOptionVO>> getBedListByRoom(
            @Parameter(description = "病房ID", example = "1", required = true)
            @RequestParam("roomId") Long roomId) {
        return JsonVO.success(null);
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
        return JsonVO.success(null);
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
            @Parameter(description = "床位ID", example = "1", required = true)
            @PathVariable("id") Long id,
            @Valid @RequestBody BedDTO bedDTO) {
        return JsonVO.success(null);
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
    public JsonVO<String> deleteBed(
            @Parameter(description = "床位ID", example = "1", required = true)
            @PathVariable("id") Long id) {
        return JsonVO.success(null);
    }
}
