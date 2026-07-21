package com.zeroone.star.j3.storehouse.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseDTO;
import com.zeroone.star.project.j3.storehouse.StoreHouseApis;
import com.zeroone.star.project.query.j3.storehouse.StoreHouseQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.storehouse.StoreHouseVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：库房药房管理控制器（文档桩实现，仅用于生成前后端交互文档）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author mie
 * @version 1.0.0
 */
@RestController
@RequestMapping("/storehouses")
@Tag(name = "库房药房管理", description = "库房药房相关操作接口")
public class StorehouseController implements StoreHouseApis {

    private static List<StoreHouseVO> buildMockData() {
        List<StoreHouseVO> list = new ArrayList<>();

        StoreHouseVO vo1 = new StoreHouseVO();
        vo1.setId(1L);
        vo1.setName("西药库");
        vo1.setType("仓库");
        vo1.setSuperiorName(null);
        vo1.setIsEnabled((byte) 1);
        list.add(vo1);

        StoreHouseVO vo2 = new StoreHouseVO();
        vo2.setId(2L);
        vo2.setName("中药房");
        vo2.setType("药房");
        vo2.setSuperiorName("住院内科");
        vo2.setIsEnabled((byte) 1);
        list.add(vo2);

        StoreHouseVO vo3 = new StoreHouseVO();
        vo3.setId(3L);
        vo3.setName("住院药房");
        vo3.setType("科室药房");
        vo3.setSuperiorName("住院部");
        vo3.setIsEnabled((byte) 0);
        list.add(vo3);

        return list;
    }

    @Override
    @GetMapping("/names")
    @Operation(summary = "获取库房药房名称（支持类型筛选）", description = "传type则按类型筛选，传null返回全部")
    public JsonVO<List<StoreHouseVO>> getStoreHouseNames(@RequestParam(required = false) Long type) {
        List<StoreHouseVO> all = buildMockData();
        List<StoreHouseVO> result;
        if (type == null) {
            result = all;
        } else {
            String typeStr = switch (type.intValue()) {
                case 1 -> "仓库";
                case 2 -> "药房";
                case 3 -> "科室药房";
                default -> "未知";
            };
            result = all.stream()
                    .filter(v -> typeStr.equals(v.getType()))
                    .collect(Collectors.toList());
        }
        return JsonVO.success(result);
    }

    @Override
    @GetMapping("/list")
    @Operation(summary = "获取库房药房列表（条件+分页）", description = "支持按名称模糊搜索、按类型筛选，返回分页结果")
    public JsonVO<PageDTO<StoreHouseVO>> list(StoreHouseQuery query) {
        List<StoreHouseVO> all = buildMockData();
        List<StoreHouseVO> filtered = all;

        if (query.getKeyword() != null && !query.getKeyword().isBlank()) {
            String kw = query.getKeyword().toLowerCase();
            filtered = filtered.stream()
                    .filter(v -> v.getName().toLowerCase().contains(kw))
                    .collect(Collectors.toList());
        }

        if (query.getType() != null) {
            String typeStr = switch (query.getType().intValue()) {
                case 1 -> "仓库";
                case 2 -> "药房";
                case 3 -> "科室药房";
                default -> "未知";
            };
            filtered = filtered.stream()
                    .filter(v -> typeStr.equals(v.getType()))
                    .collect(Collectors.toList());
        }

        PageDTO<StoreHouseVO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(query.getPageIndex() > 0 ? query.getPageIndex() : 1L);
        pageDTO.setPageSize(query.getPageSize() > 0 ? query.getPageSize() : 10L);
        pageDTO.setTotal((long) filtered.size());
        pageDTO.setPages(1L);
        pageDTO.setRows(filtered);
        return JsonVO.success(pageDTO);
    }

    @Override
    @PostMapping("/add")
    @Operation(summary = "添加库房药房", description = "新增一条库房药房记录")
    public JsonVO<String> add(@Valid @RequestBody StoreHouseDTO dto) {
        if (dto.getName() == null || dto.getName().isBlank()) {
            return JsonVO.fail("库房药房名称不能为空");
        }
        if (dto.getType() == null) {
            return JsonVO.fail("仓库类型不能为空");
        }
        List<StoreHouseVO> all = buildMockData();
        boolean exists = all.stream().anyMatch(v -> v.getName().equals(dto.getName()));
        if (exists) {
            return JsonVO.fail("库房药房名称已存在，请重新输入");
        }
        // TODO: 实际实现后续由开发者补充
        return JsonVO.success("添加成功");
    }

    @Override
    @PutMapping("/update")
    @Operation(summary = "修改库房药房", description = "根据ID修改库房药房信息")
    public JsonVO<String> update(@Valid @RequestBody StoreHouseDTO dto) {
        if (dto.getId() == null) {
            return JsonVO.fail("库房药房ID不能为空");
        }
        List<StoreHouseVO> all = buildMockData();
        boolean found = all.stream().anyMatch(v -> v.getId().equals(dto.getId()));
        if (!found) {
            return JsonVO.fail("修改失败，记录不存在");
        }
        // TODO: 实际实现后续由开发者补充
        return JsonVO.success("修改成功");
    }

    @Override
    @DeleteMapping("/delete/{id}")
    @Operation(summary = "删除库房药房", description = "根据ID删除库房药房记录")
    public JsonVO<String> delete(@PathVariable Long id) {
        if (id == null || id <= 0) {
            return JsonVO.fail("ID参数错误");
        }
        // 模拟：ID=1 的数据被引用，不能删除
        if (id == 1L) {
            return JsonVO.fail("该库房药房已被使用，无法删除");
        }
        // TODO: 实际实现后续由开发者补充
        return JsonVO.success("删除成功");
    }

    @Override
    @PutMapping("/toggleEnabled")
    @Operation(summary = "启/停用库房药房", description = "切换库房药房的启用状态")
    public JsonVO<String> toggleEnabled(@RequestParam Long id, @RequestParam Byte isEnabled) {
        if (isEnabled != 0 && isEnabled != 1) {
            return JsonVO.fail("状态参数错误，只能为 0（停用）或 1（启用）");
        }
        List<StoreHouseVO> all = buildMockData();
        boolean found = all.stream().anyMatch(v -> v.getId().equals(id));
        if (!found) {
            return JsonVO.fail("操作失败，记录不存在");
        }
        // TODO: 实际实现后续由开发者补充
        String statusText = isEnabled == 1 ? "启用" : "停用";
        return JsonVO.success(statusText + "成功");
    }
}
