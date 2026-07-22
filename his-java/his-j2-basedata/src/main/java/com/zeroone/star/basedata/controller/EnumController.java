package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.*;
import com.zeroone.star.project.j2.basedata.nulll.EnumApis;
import com.zeroone.star.project.query.j2.basedata.EnumListQuery;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@Tag(name = "枚举管理相关接口")
public class EnumController implements EnumApis {

    // TODO: 这里按理说应该传递id来删除对应实体，只有一个值就不用包装了吧
    @Override
    @Operation(summary = "删除枚举类型")
    @DeleteMapping("/sys/delete-enum")
    public JsonVO<String> deleteEnumType(@PathVariable EnumDeleteDTO dto) {
        return null;
    }

    @Override
    @Operation(summary = "获取枚举名称列表")
    @GetMapping("/sys/get-enum-type-name")
    public JsonVO<List<String>> getEnumTypeName(@RequestBody EnumListQuery query) {
        return null;
    }


    @Override
    @Operation(summary = "获取所有枚举类型列表(条件+分页)")
    @GetMapping("/sys/query-all-enum")
    public JsonVO<PageDTO<EnumDTO>> queryAllEnum(@RequestBody EnumQuery query) {
        return null;
    }

    @Override
    @Operation(summary = "根据枚举类型查询枚举列表")
    @GetMapping("/sys/list-enums-by-enum-type/{enumTypeId}")
    public JsonVO<List<EnumDTO>> listEnumsByEnumType(@PathVariable(value = "enumTypeId", required = true) String enumTypeId) {
        return null;
    }

    @GetMapping("/sys/query-all-enum-type")
    @Operation(summary = "分页查询枚举类型", description = "主要是分页，这里当前只有依据关键搜索")
    @Override
    public JsonVO<PageDTO<EnumTypeDTO>> queryAllEnumType(@RequestBody @Valid EnumTypeQuery query) {
        return null;
    }

    @PostMapping("/sys/add-enum-type")
    @Operation(summary = "创建枚举类型", description = "租户等信息由jwt等提供，id自动生成")
    @Override
    public JsonVO<String> addEnumType(@RequestBody @Valid EnumTypeAddDTO data) {
        return null;
    }

    @PutMapping("/sys/modify-enum-type")
    @Operation(summary = "修改枚举类型", description = "对于创建过程，需要额外提供id")
    @Override
    public JsonVO<String> modifyEnumType(@RequestBody @Valid EnumTypeModifyDTO data) {
        return null;
    }
}
