package com.zeroone.star.project.j2.dictionarytype;

import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryModifyDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * 字典项管理接口。
 */
public interface DictionaryApis {

    /**
     * 新增字典项。
     *
     * @param data 新增参数
     * @return 新增字典项ID
     */
    JsonVO<String> addDictionary(DictionaryAddDTO data);

    /**
     * 修改字典项。
     *
     * @param data 修改参数
     * @return 修改字典项ID
     */
    JsonVO<String> modifyDictionary(DictionaryModifyDTO data);

    /**
     * 逻辑删除或停用字典项。
     *
     * @param data 删除参数
     * @return 处理后的字典项ID
     */
    JsonVO<String> removeDictionary(DictionaryDeleteDTO data);
}
