package com.zeroone.star.project.j2.basedata.dictionary;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDTO;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;
import com.zeroone.star.project.vo.JsonVO;

public interface DictionaryTypeApis {

    /**
     * 获取字典类型列表（条件+分页）
     *
     * @param query 字典类型查询条件
     * @return 字典类型分页列表
     */
    JsonVO<PageDTO<DictionaryTypeDTO>> queryDictionaryType(DictionaryTypeQuery query);
}
