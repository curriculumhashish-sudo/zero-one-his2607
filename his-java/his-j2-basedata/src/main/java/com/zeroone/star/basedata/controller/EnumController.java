package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.EnumTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.*;
import com.zeroone.star.project.j2.basedata.nulll.EnumApis;
import com.zeroone.star.project.query.j2.basedata.EnumListQuery;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;

@RestController
@Tag(name = "枚举管理")
public class EnumController implements EnumApis {

    @Resource
    private EnumTypeService enumTypeService;


    @Override
    @Operation(summary = "获取枚举名称列表")
    @GetMapping("/sys/get-enum-type-name")
    public JsonVO<List<String>> getEnumTypeName(@RequestBody EnumListQuery query) {
        return null;
    }

    @Override
    @Operation(summary = "根据枚举类型id查询枚举列表")
    @GetMapping("/sys/list-enums-by-enum-type/{enumTypeId}")
    public JsonVO<List<EnumDTO>> listEnumsByEnumType(@PathVariable(value = "enumTypeId", required = true) String enumTypeId) {
        return null;
    }

    @Override
    @Operation(summary = "根据枚举类型key查询枚举列表", description = "注意这里使用的是key")
    @GetMapping("/sys/init-table-enum")
    public JsonVO<Map<String, List<EnumDTO>>> initTableEnums(@RequestParam List<String> enumTypeKeys) {
        return null;
    }


    @Override
    @Operation(summary = "获取枚举类型列表(条件+分页)")
    @GetMapping("/sys/query-all-enum-type")
    public JsonVO<PageDTO<EnumTypeDTO>> queryAllEnumType(@RequestBody EnumTypeQuery query) {
        return JsonVO.success(enumTypeService.queryAllEnumType(query));
    }

    @PostMapping("/sys/add-enum")
    @Operation(summary = "添加枚举", description = "租户等信息由jwt等提供，id自动生成")
    @Override
    public JsonVO<String> addEnum(@RequestBody @Valid EnumAddDTO data) {
        return null;
    }

    @PutMapping("/sys/modify-enum")
    @Operation(summary = "修改枚举", description = "租户等信息由jwt等提供，id自动生成")
    @Override
    public JsonVO<String> modifyEnum(@RequestBody @Valid EnumModifyDTO data) {
        return null;
    }

    @DeleteMapping("/sys/delete-enum/{id}")
    @Operation(summary = "删除枚举", description = "租户等信息由jwt等提供，id自动生成")
    @Override
    public JsonVO<String> deleteEnum(@PathVariable(value = "id", required = true) String id) {
        return null;
    }


    @PostMapping("/sys/add-enum-type")
    @Operation(summary = "创建枚举类型", description = "租户等信息由jwt等提供，id自动生成")
    @Override
    public JsonVO<String> addEnumType(@RequestBody @Valid EnumTypeAddDTO data) {
        enumTypeService.saveEnumType(data);
        return JsonVO.success("success");
    }

    @PutMapping("/sys/modify-enum-type")
    @Operation(summary = "修改枚举类型", description = "对于创建过程，需要额外提供id")
    @Override
    public JsonVO<String> modifyEnumType(@RequestBody @Valid EnumTypeModifyDTO data) {
        enumTypeService.updateEnumType(data);

        return JsonVO.success("success");
    }

    @Override
    @Operation(summary = "删除枚举类型")
    @DeleteMapping("/sys/delete-enum")
    public JsonVO<Integer> deleteEnumType(@PathVariable EnumDeleteDTO dto) {
        return null;
    }
}
