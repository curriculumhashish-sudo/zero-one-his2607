package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryDTO;
import com.zeroone.star.project.j2.basedata.dictionary.DictionaryTypeApis;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Tag(name = "字典类型管理")
public class DictionaryTypeController implements DictionaryTypeApis {

    @Operation(summary = "获取字典列表（条件+分页）")
    @GetMapping("dictionary/query-all")
    @Override
    public JsonVO<PageDTO<DictionaryDTO>> queryDictionaryType(DictionaryTypeQuery query) {
        return null;
    }
}
