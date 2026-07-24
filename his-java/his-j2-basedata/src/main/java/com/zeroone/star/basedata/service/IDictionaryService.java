package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.dictionarytype.DictionaryModifyDTO;

/**
 * 字典项业务接口。
 */
public interface IDictionaryService {

    String addDictionary(DictionaryAddDTO data);

    String modifyDictionary(DictionaryModifyDTO data);

    String removeDictionary(DictionaryDeleteDTO data);
}
