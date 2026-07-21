package com.zeroone.star.project.j2.dictionarytype;

import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryModifyDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

/**
 * 字典项管理接口。
 */
@Tag(name = "字典项管理")
public interface DictionaryApis {

    /**
     * 新增字典项。
     *
     * @param data 新增参数
     * @return 新增字典项ID
     */
    @Operation(summary = "新增字典项")
    @PostMapping("/addConcept")
    JsonVO<String> addDictionary(@Valid @RequestBody DictionaryAddDTO data);

    /**
     * 修改字典项。
     *
     * @param data 修改参数
     * @return 修改字典项ID
     */
    @Operation(summary = "修改字典项")
    @PostMapping("/updateConcept")
    JsonVO<String> modifyDictionary(@Valid @RequestBody DictionaryModifyDTO data);

    /**
     * 删除字典项。
     *
     * @param data 删除参数
     * @return 处理后的字典项ID
     */
    @Operation(summary = "删除字典项")
    @PostMapping("/deleteConcept")
    JsonVO<String> removeDictionary(@Valid @RequestBody DictionaryDeleteDTO data);
}
