package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDeleteDTO;
import com.zeroone.star.project.j2.basedata.enumApis;
import com.zeroone.star.project.query.j2.basedata.EnumListQuery;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;

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
}
