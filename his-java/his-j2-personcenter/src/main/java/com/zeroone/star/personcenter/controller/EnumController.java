package com.zeroone.star.personcenter.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDeleteDTO;
import com.zeroone.star.project.j2.sys.enumApis;
import com.zeroone.star.project.query.j2.sys.EnumListQuery;
import com.zeroone.star.project.query.j2.sys.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@Tag(name = "枚举管理相关接口")
public class EnumController implements enumApis {
    @Resource
    enumApis enumApis;

    @Override
    @Operation(summary = "删除枚举")
    @DeleteMapping("/sys/delete-enum")
    public JsonVO<Integer> deleteEnum(@RequestBody EnumDeleteDTO dto) {
        return null;
    }

    @Override
    @Operation(summary = "获取枚举名称列表")
    @GetMapping("/sys/get-enum-name-list/{enumKey}")
    public JsonVO<List<String>> getEnumNameList(@PathVariable String enumKey) {
        return enumApis.getEnumNameList(enumKey);
    }

    @Override
    @Operation(summary = "获取枚举列表(条件+分页)")
    @GetMapping("/sys/query-all-enum")
    public JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query) {
        return enumApis.QueryAllEnum(query);
    }

    @Override
    @Operation(summary = "删除枚举类型")
    @DeleteMapping("/sys/delete-enum-type/{enumKey}")
    public JsonVO<String> deleteEnumType(@PathVariable(value = "enumKey", required = true) String enumKey) {
        return enumApis.deleteEnumType(enumKey);
    }
}
