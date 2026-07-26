package com.zeroone.star.project.j3.ward;

import com.zeroone.star.project.dto.j3.ward.*;
import com.zeroone.star.project.query.j3.ward.*;
import com.zeroone.star.project.vo.JsonVO;
import jakarta.validation.Valid;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;

import java.util.List;

/**
 * <p>
 * 描述：病区病房接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Tag(name = "病区病房管理", description = "病区-病房-床位相关操作接口")
public interface WardApis {

    /** ==================== 树形结构 ==================== */

    @Operation(summary = "获取病区-病房-床位树形列表", description = "根据条件机构名称获取树形结构数据")
    JsonVO<List<WardTreeQuery>> getWardTree(
            @Parameter(description = "树形查询条件", required = true)
            WardTreeQuery query
    );

    /** ==================== 病区管理 ==================== */

    @Operation(summary = "获取病区列表", description = "获取所有病区名称列表（用于下拉框）")
    JsonVO<List<WardQuery>> getWardList();

    @Operation(summary = "添加病区", description = "新增一条病区记录")
    JsonVO<String> addWard(
            @Parameter(description = "病区添加数据", required = true)
            WardAddDTO wardAddDTO
    );

    @Operation(summary = "修改病区", description = "根据ID修改病区信息")
    JsonVO<String> updateWard(
            @Parameter(description = "病区ID", example = "1", required = true)
            Long id,
            @Parameter(description = "病区数据", required = true)
            WardDTO wardDTO
    );

    @Operation(summary = "删除病区", description = "根据ID删除病区记录")
    JsonVO<String> deleteWard(
            @Parameter(description = "病区ID", example = "1", required = true)
            Long id
    );

    /** ==================== 病房管理 ==================== */

    @Operation(summary = "根据病区获取病房列表", description = "根据病区获取病房名称列表")
    JsonVO<List<RoomQuery>> getRoomListByWard(
            @Parameter(description = "病区ID", example = "1", required = true)
            Long wardId
    );

    @Operation(summary = "添加病房", description = "新增一条病房记录")
    JsonVO<String> addRoom(
            @Parameter(description = "病房添加数据", required = true)
            RoomAddDTO roomAddDTO
    );

    @Operation(summary = "修改病房", description = "根据ID修改病房信息")
    JsonVO<String> updateRoom(
            @Parameter(description = "病房ID", example = "1", required = true)
            Long id,
            @Parameter(description = "病房数据", required = true)
            RoomDTO roomDTO
    );

    @Operation(summary = "删除病房", description = "根据ID删除病房记录")
    JsonVO<String> deleteRoom(
            @Parameter(description = "病房ID", example = "1", required = true)
            Long id
    );

    /** ==================== 床位管理 ==================== */

    @Operation(summary = "根据病房获取病床列表", description = "根据病房获取病床名称，包含名称、唯一标识、状态")
    JsonVO<List<BedQuery>> getBedListByRoom(
            @Parameter(description = "病房ID", example = "1", required = true)
            Long roomId
    );

    @Operation(summary = "添加床位", description = "新增一条床位记录")
    JsonVO<String> addBed(
            @Parameter(description = "床位添加数据", required = true)
            BedAddDTO bedAddDTO
    );

    @Operation(summary = "修改床位", description = "根据ID修改床位信息")
    JsonVO<String> updateBed(
            @Parameter(description = "床位ID", example = "1", required = true)
            Long id,
            @Parameter(description = "床位数据", required = true)
            BedDTO bedDTO
    );

    @Operation(summary = "删除床位", description = "根据ID删除床位记录")
    JsonVO<String> deleteBed(
            @Parameter(description = "床位ID", example = "1", required = true)
            Long id
    );
}
