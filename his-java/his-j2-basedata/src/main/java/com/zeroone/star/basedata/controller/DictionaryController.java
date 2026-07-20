package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryModifyDTO;
import com.zeroone.star.project.j2.dictionarytype.DictionaryApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j2.dictionarytype.DictionaryVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * 字典项管理控制器。
 */
@RestController
@RequestMapping("/dictionary")
@Tag(name = "字典项管理")
public class DictionaryController implements DictionaryApis {

    @Override
    @PostMapping("/addConcept")
    @Operation(summary = "新增字典项")
    public JsonVO<DictionaryVO> addDictionary(@Valid @RequestBody DictionaryAddDTO data) {
        return null;
    }

    @Override
    @PostMapping("/updateConcept")
    @Operation(summary = "修改字典项")
    public JsonVO<DictionaryVO> modifyDictionary(@Valid @RequestBody DictionaryModifyDTO data) {
        return null;
    }

    @Override
    @PostMapping("/deleteConcept")
    @Operation(summary = "删除或停用字典项")
    public JsonVO<DictionaryVO> removeDictionary(@Valid @RequestBody DictionaryDeleteDTO data) {
        return null;
    }
}
