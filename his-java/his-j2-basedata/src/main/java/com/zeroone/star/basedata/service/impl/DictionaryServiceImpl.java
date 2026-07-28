package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.DictionaryConceptDO;
import com.zeroone.star.basedata.exception.DictionaryBusinessException;
import com.zeroone.star.basedata.mapper.DictionaryMapper;
import com.zeroone.star.basedata.service.IDictionaryService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryModifyDTO;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

/**
 * 字典项业务实现。
 */
@Service
@RequiredArgsConstructor

public class DictionaryServiceImpl implements IDictionaryService {

    private final DictionaryMapper dictionaryMapper;

    private DictionaryServiceImpl dictionaryService;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String addDictionary(DictionaryAddDTO data) {
        String codeSystemId = data.getCodeSystemId().trim();
        String code = data.getCode().trim();
        checkCodeSystem(codeSystemId);
        checkCodeUnique(codeSystemId, code, null);

        DictionaryConceptDO concept = new DictionaryConceptDO();
        concept.setCodeSystemId(codeSystemId);
        concept.setCode(code);
        concept.setDisplay(data.getDisplay().trim());
        concept.setDefinition(data.getDefinition().trim());
        concept.setSortOrder(resolveSortOrder(codeSystemId, data.getSortOrder()));
        concept.setIsActive(true);
        concept.setIsDeleted(false);

        if (dictionaryMapper.insert(concept) != 1) {
            throw new DictionaryBusinessException("新增字典项失败");
        }
        return concept.getId();
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String modifyDictionary(DictionaryModifyDTO data) {
        String id = data.getId().trim();
        if (dictionaryMapper.countConcept(id) == 0) {
            throw new DictionaryBusinessException("字典项不存在或已删除");
        }

        String codeSystemId = data.getCodeSystemId().trim();
        String code = data.getCode().trim();
        checkCodeSystem(codeSystemId);
        checkCodeUnique(codeSystemId, code, id);

        DictionaryConceptDO concept = new DictionaryConceptDO();
        concept.setId(id);
        concept.setCodeSystemId(codeSystemId);
        concept.setCode(code);
        concept.setDisplay(data.getDisplay().trim());
        concept.setDefinition(data.getDefinition().trim());
        concept.setSortOrder(data.getSortOrder());

        if (dictionaryMapper.updateConcept(concept) != 1) {
            throw new DictionaryBusinessException("修改字典项失败，请刷新后重试");
        }
        return id;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String removeDictionary(DictionaryDeleteDTO data) {
        String id = data.getId().trim();
        DictionaryConceptDO concept = new DictionaryConceptDO();
        concept.setId(id);
        concept.setIsDeleted(true);
        if (dictionaryMapper.logicallyDelete(concept) != 1) {
            throw new DictionaryBusinessException("字典项不存在或已删除");
        }
        return id;
    }

    private void checkCodeSystem(String codeSystemId) {
        if (dictionaryMapper.lockCodeSystem(codeSystemId) == null) {
            throw new DictionaryBusinessException("字典类型不存在或已删除");
        }
    }

    private void checkCodeUnique(String codeSystemId, String code, String excludeId) {
        if (dictionaryMapper.countDuplicateCode(codeSystemId, code, excludeId) > 0) {
            throw new DictionaryBusinessException("同一字典类型下字典编码不能重复");
        }
    }

    private Long resolveSortOrder(String codeSystemId, Long sortOrder) {
        if (sortOrder != null) {
            return sortOrder;
        }
        Long maxSortOrder = dictionaryMapper.selectMaxSortOrder(codeSystemId);
        if (maxSortOrder == null || maxSortOrder < 0) {
            return 0L;
        }
        if (maxSortOrder == Long.MAX_VALUE) {
            throw new DictionaryBusinessException("排序号已达到最大值，请手动指定排序号");
        }
        return maxSortOrder + 1;
    }
}
