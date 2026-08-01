package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryModifyDTO;

/**
 * 字典项业务接口。
 */
public interface IDictionaryService {

    String addDictionary(DictionaryAddDTO data);

    String modifyDictionary(DictionaryModifyDTO data);

    String removeDictionary(DictionaryDeleteDTO data);
}
