package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.*;
import com.zeroone.star.project.j2.basedata.nulll.enumApis;
import com.zeroone.star.project.query.j2.basedata.EnumListQuery;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@Tag(name = "枚举管理相关接口")
public class EnumController implements enumApis {
    @Override
    @Operation(summary = "删除枚举类型")
    @DeleteMapping("/sys/delete-enum")
    public JsonVO<String> deleteEnumType(@PathVariable EnumDeleteDTO dto) {
        return null;
    }

    @Override
    @Operation(summary = "获取枚举名称列表")
    @GetMapping("/sys/get-enum-type-name")
    public JsonVO<List<String>> getEnumTypeName(EnumListQuery query) {
        return null;
    }

    @Override
    @Operation(summary = "获取所有枚举类型列表(条件+分页)")
    @GetMapping("/sys/query-all-enum")
    public JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query) {
        return null;
    }

    @GetMapping("/query-all")
    @Operation(summary = "分页查询枚举类型", description = "分页查询枚举类型")
    @Override
    public JsonVO<PageDTO<EnumTypeDTO>> queryAll(EnumTypeQuery query) {
        return null;
    }

    @PostMapping("/add-enum-type")
    @Operation(summary = "添加枚举类型", description = "添加枚举类型")
    @Override
    public JsonVO<String> addEnumType(EnumTypeAddDTO data) {
        return null;
    }

    @PutMapping("/modify-enum-type")
    @Operation(summary = "修改枚举类型", description = "修改枚举类型")
    @Override
    public JsonVO<String> modifyEnumType(EnumTypeModifyDTO data) {
        return null;
    }
}
