package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.mapper.FieldMapper;
import com.zeroone.star.configuration.service.FieldService;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldAddDTO;
import com.zeroone.star.project.dto.j5.medicalTemplate.FieldUpdateDTO;
import com.zeroone.star.project.j5.medicalTemplate.FieldApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.enums.ParameterIn;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.UUID;

/**
 * 描述：配置管理-病历模板：元数据字段管理控制器
 * 作者：青梅不甜
 */
@RestController
@Tag(name = "元数据字段管理")
public class FieldController implements FieldApis {

    @Autowired
    private FieldService fieldService;
    @Resource
    private FieldMapper fieldMapper;

    @Operation(summary = "添加元数据字段")
    @PostMapping("/field")
    @Override
    public JsonVO<String> addField(@RequestBody FieldAddDTO data) {
        int result = fieldService.addField(data);
        if(result <= 0){
            return JsonVO.fail("添加失败");
        }
        return JsonVO.success("添加成功");
    }

    @Operation(summary = "修改元数据字段")
    @PutMapping("/field")
    @Override
    public JsonVO<String> updateField(@RequestBody FieldUpdateDTO data) {
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
        return JsonVO.success(id);
    }
}