package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.FieldService;
import com.zeroone.star.configuration.service.MetadataService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.Metadata.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.Metadata.MetadataListDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldBaseDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.j5.configuration.MetadataApis;
import com.zeroone.star.project.j5.medicalTemplate.FieldApis;
import com.zeroone.star.project.query.j5.Metadata.MetadataQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.enums.ParameterIn;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 描述：配置管理-病历模板：元数据字段管理控制器
 * 作者：青梅不甜
 */
@RestController
@Validated
@Tag(name = "配置管理-元数据字段", description = "元数据字段管理接口")
public class FieldController implements MetadataApis, FieldApis {

    @Autowired
    private FieldService fieldService;

    @Autowired
    private MetadataService metadataService;

    @Operation(summary = "获取元数据字段名称")
    @GetMapping("/metadata/field-names")
    @Override
    public JsonVO<List<String>> listFieldNames() {
        return JsonVO.success(metadataService.listFieldNames());
    }

    @Operation(summary = "获取元数据字段列表（分页）")
    @GetMapping("/metadata/fields")
    @Override
    public JsonVO<PageDTO<MetadataListDTO>> listFields(@Validated MetadataQuery query) {
        return JsonVO.success(metadataService.listFields(query));
    }

    @Operation(summary = "获取元数据字段详情")
    @GetMapping("/metadata/fields/{id}")
    @Parameter(name = "id", description = "字段ID", required = true, in = ParameterIn.PATH, example = "1d7b0520e93e72715d5d6af1fb7d9a37")
    @Override
    public JsonVO<MetadataDetailDTO> getFieldDetail(@PathVariable String id) {
        return JsonVO.success(metadataService.getFieldDetail(id));
    }

    @Operation(summary = "添加元数据字段")
    @PostMapping("/field")
    @Override
    public JsonVO<String> addField(@Valid @RequestBody FieldBaseDTO data) {
        int result = fieldService.addField(data);
        if (result <= 0) {
            return JsonVO.fail("添加失败");
        }
        return JsonVO.success("添加成功");
    }

    @Operation(summary = "修改元数据字段")
    @PutMapping("/field")
    @Override
    public JsonVO<String> updateField(@Valid @RequestBody FieldUpdateDTO data) {
        int result = fieldService.updateField(data);
        if (result <= 0) {
            return JsonVO.fail("修改失败");
        }
        return JsonVO.success("修改成功");
    }

    @Operation(summary = "删除元数据字段")
    @Parameter(name = "id", description = "字段ID", required = true, in = ParameterIn.PATH, example = "1d7b0520e93e72715d5d6af1fb7d9a37")
    @DeleteMapping("/field/{id}")
    @Override
    public JsonVO<String> delField(@PathVariable String id) {
        int result = fieldService.deleteField(id);
        if (result <= 0) {
            return JsonVO.fail("删除失败");
        }
        return JsonVO.success(id);
    }
}
