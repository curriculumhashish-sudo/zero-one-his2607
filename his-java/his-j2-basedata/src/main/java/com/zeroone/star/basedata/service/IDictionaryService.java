package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryModifyDTO;

/**
 * 字典项业务接口。
 */
public interface IDictionaryService {

    String addDictionary(DictionaryAddDTO data);

    String modifyDictionary(DictionaryModifyDTO data);

    String removeDictionary(DictionaryDeleteDTO data);
}
