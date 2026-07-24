package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.DictionaryTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDTO;
import com.zeroone.star.project.j2.basedata.dictionary.DictionaryTypeApis;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Tag(name = "字典类型管理")
@RequiredArgsConstructor
public class DictionaryTypeController implements DictionaryTypeApis {

    private final DictionaryTypeService dictionaryTypeService;

    @Operation(summary = "获取字典类型列表（条件+分页）")
    @GetMapping("dictionary/query-all")
    @Override
    public JsonVO<PageDTO<DictionaryTypeDTO>> queryDictionaryType(DictionaryTypeQuery query) {

        PageDTO<DictionaryTypeDTO> pageResult = dictionaryTypeService.queryDictionarType(query);

        return JsonVO.success(pageResult);
    }
}
