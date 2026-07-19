package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.EnumTypeAddDTO;
import com.zeroone.star.project.dto.j2.basedata.EnumTypeDTO;
import com.zeroone.star.project.dto.j2.basedata.EnumTypeModifyDTO;
import com.zeroone.star.project.login.j2.basedata.EnumApis;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

@RestController
@Tag(name = "枚举管理", description = "枚举管理相关接口")
public class EnumController implements EnumApis {

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
