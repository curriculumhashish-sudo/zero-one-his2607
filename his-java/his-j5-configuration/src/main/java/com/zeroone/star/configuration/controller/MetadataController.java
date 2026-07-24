package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.MetadataService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataListDTO;
import com.zeroone.star.project.j5.configuration.MetadataApis;
import com.zeroone.star.project.query.j5.configuration.MetadataQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.enums.ParameterIn;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/metadata")
@Tag(name = "元数据字段管理")
@Validated
public class MetadataController implements MetadataApis {

    @Resource
    MetadataService service;

    @GetMapping("/field-names")
    @Operation(summary = "获取元数据字段名称")
    @Override
    public JsonVO<List<String>> listFieldNames() {
        return JsonVO.success(service.listFieldNames());
    }

    @GetMapping("/fields")
    @Operation(summary = "获取元数据字段列表（分页）")
    @Override
    public JsonVO<PageDTO<MetadataListDTO>> listFields(@Validated MetadataQuery query) {
        return JsonVO.success(service.listFields(query));
    }

    @GetMapping("/fields/{id}")
    @Operation(summary = "获取元数据字段详情")
    @Parameter(name = "id", description = "字段ID", required = true, in = ParameterIn.PATH, example = "1d7b0520e93e72715d5d6af1fb7d9a37")
    @Override
    public JsonVO<MetadataDetailDTO> getFieldDetail(@PathVariable String id) {
        return JsonVO.success(service.getFieldDetail(id));
    }
}