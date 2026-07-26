package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.exception.DictionaryBusinessException;
import com.zeroone.star.basedata.service.DictionaryTypeService;
import com.zeroone.star.basedata.service.IDictionaryService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.*;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemDTO;
import com.zeroone.star.project.j2.basedata.dictionary.DictItemApis;
import com.zeroone.star.project.j2.basedata.dictionary.DictionaryApis;
import com.zeroone.star.project.j2.basedata.dictionary.DictionaryTypeApis;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;
import com.zeroone.star.project.query.j2.sysmgr.DictItemQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.function.Supplier;

/**
 * <p>
 * 描述：字典管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@RequestMapping("/dictionary")
@Tag(name = "字典管理")
@RequiredArgsConstructor
public class DictItemController implements DictItemApis, DictionaryApis, DictionaryTypeApis {

    private final IDictionaryService dictionaryService;

    private final DictionaryTypeService dictionaryTypeService;

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

    private JsonVO<String> execute(Supplier<String> operation) {
        try {
            return JsonVO.success(operation.get());
        } catch (DictionaryBusinessException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Operation(summary = "获取字典类型列表（条件+分页）")
    @GetMapping("dictionary/query-all")
    @Override
    public JsonVO<PageDTO<DictionaryTypeDTO>> queryDictionaryType(DictionaryTypeQuery query) {

        PageDTO<DictionaryTypeDTO> pageResult = dictionaryTypeService.queryDictionarType(query);

        return JsonVO.success(pageResult);
    }

    @Operation(summary = "获取字典名称列表")
    @GetMapping("/queryDictNameList")
    @Override
    public JsonVO<List<String>> queryDictNameList(DictItemQuery query) {
        return null;
    }

    @Operation(summary = "获取字典列表(条件+分页)",description = "获取字典列表(条件+分页)")
    @GetMapping("/queryDictItem")
    @Override
    public JsonVO<PageDTO<DictItemDTO>> queryAll(DictItemQuery query) {
        return null;
    }
    @Operation(summary = "新增字典")
    @PostMapping("/addDictItem")
    @Override
    public JsonVO<String> addDictItem(@RequestBody DictItemAddDTO addDTO) {
        return null;
    }
    @Operation(summary = "修改字典")
    @PutMapping("/dictItem")
    @Override
    public JsonVO<String> updateDict(@RequestBody DictItemUpdateDTO updateDTO) {
        return null;
    }

    @Operation(summary = "删除字典")
    @DeleteMapping("/dictItem/{id}")
    @Override
    public JsonVO<String> deleteDict(@PathVariable("id") Long id) {
        return null;
    }

}
