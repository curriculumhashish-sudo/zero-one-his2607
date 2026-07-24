package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.exception.DictionaryBusinessException;
import com.zeroone.star.basedata.service.IDictionaryService;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryModifyDTO;
import com.zeroone.star.project.j2.dictionarytype.DictionaryApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.function.Supplier;

/**
 * 字典项管理控制器。
 */
@RestController
@RequestMapping("/dictionary")
@Tag(name = "字典项管理")
@RequiredArgsConstructor
public class DictionaryController implements DictionaryApis {

    private final IDictionaryService dictionaryService;

    @Override
    @PostMapping("/addConcept")
    @Operation(summary = "新增字典项")
    public JsonVO<String> addDictionary(@Valid @RequestBody DictionaryAddDTO data) {
        return execute(() -> dictionaryService.addDictionary(data));
    }

    @Override
    @PostMapping("/updateConcept")
    @Operation(summary = "修改字典项")
    public JsonVO<String> modifyDictionary(@Valid @RequestBody DictionaryModifyDTO data) {
        return execute(() -> dictionaryService.modifyDictionary(data));
    }

    @Override
    @PostMapping("/deleteConcept")
    @Operation(summary = "删除字典项")
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
}
