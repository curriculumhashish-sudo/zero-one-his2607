package com.zeroone.star.configuration.controller;

import com.zeroone.star.project.dto.j5.medicalTemplate.FieldAddDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.j5.medicalTemplate.FieldApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

/**
 * 描述：元数据字段管理控制器
 * 作者：青梅不甜
 */
@RestController
@Tag(name = "元数据字段管理")
public class FieldController implements FieldApis {

    @Operation(summary = "添加元数据字段")
    @PostMapping("/field")
    @Override
    public JsonVO<String> addField(@RequestBody FieldAddDTO data) {
        return null;
    }

    @Operation(summary = "修改元数据字段")
    @PutMapping("/field")
    @Override
    public JsonVO<String> updateField(@RequestBody FieldUpdateDTO data) {
        return null;
    }

    @Operation(summary = "删除元数据字段")
    @DeleteMapping("/field/{id}")
    @Override
    public JsonVO<String> delField(@PathVariable String id) {
        return null;
    }
}
