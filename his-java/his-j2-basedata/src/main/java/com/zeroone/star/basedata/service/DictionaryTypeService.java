package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDTO;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;

public interface DictionaryTypeService {
    PageDTO<DictionaryTypeDTO> queryDictionarType(DictionaryTypeQuery query);
}
