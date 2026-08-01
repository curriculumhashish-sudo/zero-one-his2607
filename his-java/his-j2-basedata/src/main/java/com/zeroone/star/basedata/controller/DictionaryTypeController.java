package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.exception.DictionaryBusinessException;
import com.zeroone.star.basedata.service.DictionaryTypeService;
import com.zeroone.star.basedata.service.IDictionaryService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryModifyDTO;
import com.zeroone.star.project.j2.basedata.dictionary.DictionaryTypeApis;
import com.zeroone.star.project.j2.basedata.dictionarytype.DictionaryApis;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDTO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

import java.util.function.Supplier;

/**
 * 字典类型管理控制器。
 */
@RequiredArgsConstructor
@RestController
@RequestMapping("/dictionarytype")
@Tag(name = "字典类型")
public class DictionaryTypeController implements DictionaryApis, DictionaryTypeApis {

    private final DictionaryTypeService dictionaryTypeService ;
    private final IDictionaryService dictionaryService;

    @Override
    @PostMapping("/addConcept")
    @Operation(summary = "新增字典类型")
    public JsonVO<String> addDictionary(@Valid @RequestBody DictionaryAddDTO data) {
        return execute(() -> dictionaryService.addDictionary(data));
    }

    @Override
    @PostMapping("/updateConcept")
    @Operation(summary = "修改字典类型")
    public JsonVO<String> modifyDictionary(@Valid @RequestBody DictionaryModifyDTO data) {
        return execute(() -> dictionaryService.modifyDictionary(data));
    }

    @Override
    @PostMapping("/deleteConcept")
    @Operation(summary = "删除字典类型")
    public JsonVO<String> removeDictionary(@Valid @RequestBody DictionaryDeleteDTO data) {
        return execute(() -> dictionaryService.removeDictionary(data));
    }

    @Operation(summary = "获取字典类型列表（条件+分页）")
    @GetMapping("/queryall")
    @Override
    public JsonVO<PageDTO<DictionaryTypeDTO>> queryDictionaryType(DictionaryTypeQuery query) {
        PageDTO<DictionaryTypeDTO> pageResult = dictionaryTypeService.queryDictionarType(query);
        return JsonVO.success(pageResult);
    }

    private JsonVO<String> execute(Supplier<String> operation) {
        try {
            return JsonVO.success(operation.get());
        } catch (DictionaryBusinessException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
