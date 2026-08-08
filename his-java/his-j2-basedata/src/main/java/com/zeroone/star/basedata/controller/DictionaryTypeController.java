package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.DictionaryTypeService;
import com.zeroone.star.basedata.service.IDictionaryService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeCreateDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDeleteDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeUpdateDTO;
import com.zeroone.star.project.j2.basedata.dictionary.DictionaryTypeApis;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDTO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

/**
 * 字典类型管理控制器。
 */
@RequiredArgsConstructor
@RestController
@RequestMapping("/dictionarytype")
@Tag(name = "字典类型")
public class DictionaryTypeController implements DictionaryTypeApis {

    private final DictionaryTypeService dictionaryTypeService ;
    private final IDictionaryService dictionaryService;

    @Operation(summary = "获取字典类型列表（条件+分页）")
    @GetMapping("/queryall")
    @Override
    public JsonVO<PageDTO<DictionaryTypeDTO>> queryDictionaryType(DictionaryTypeQuery query) {
        PageDTO<DictionaryTypeDTO> pageResult = dictionaryTypeService.queryDictionarType(query);
        return JsonVO.success(pageResult);
    }

    @PostMapping("/addConcept")
    @Operation(summary = "新增字典类型")
    @Override
    public JsonVO<String> createDictionaryType(@Valid @RequestBody DictionaryTypeCreateDTO data) {
        return JsonVO.success(dictionaryTypeService.createDictionaryType(data));
    }


    @Override
    @PutMapping("/updateConcept")
    @Operation(summary = "修改字典类型")
    public JsonVO<String> updateDictionaryType(@Valid @RequestBody DictionaryTypeUpdateDTO data) {
        return JsonVO.success(dictionaryTypeService.updateDictionaryType(data));
    }


    @Override
    @DeleteMapping("/deleteConcept")
    @Operation(summary = "删除字典类型")
    public JsonVO<String> deleteDictionaryType(@Valid @RequestBody DictionaryTypeDeleteDTO data) {
        return JsonVO.success(dictionaryTypeService.deleteDictionaryType(data));
    }


}
