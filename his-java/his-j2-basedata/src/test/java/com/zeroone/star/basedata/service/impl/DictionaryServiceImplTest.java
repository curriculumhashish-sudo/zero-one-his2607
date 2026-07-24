package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.DictionaryConceptDO;
import com.zeroone.star.basedata.exception.DictionaryBusinessException;
import com.zeroone.star.basedata.mapper.DictionaryMapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryAddDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryDeleteDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarytypedto.DictionaryModifyDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class DictionaryServiceImplTest {

    @Mock
    private DictionaryMapper dictionaryMapper;
    @Mock
    private UserHolder userHolder;

    private DictionaryServiceImpl dictionaryService;

    @BeforeEach
    void setUp() {
        dictionaryService = new DictionaryServiceImpl(dictionaryMapper, userHolder);
    }

    @Test
    void addDictionaryShouldTrimFieldsAndAppendDefaultSortOrder() throws Exception {
        DictionaryAddDTO data = addDTO();
        data.setSortOrder(null);
        mockOperator();
        when(dictionaryMapper.lockCodeSystem("900010")).thenReturn("900010");
        when(dictionaryMapper.countDuplicateCode("900010", "1", null)).thenReturn(0);
        when(dictionaryMapper.selectMaxSortOrder("900010")).thenReturn(8L);
        when(dictionaryMapper.insert(any(DictionaryConceptDO.class))).thenAnswer(invocation -> {
            DictionaryConceptDO concept = invocation.getArgument(0);
            concept.setId("900010001");
            return 1;
        });

        String id = dictionaryService.addDictionary(data);

        assertEquals("900010001", id);
        ArgumentCaptor<DictionaryConceptDO> captor = ArgumentCaptor.forClass(DictionaryConceptDO.class);
        verify(dictionaryMapper).insert(captor.capture());
        assertEquals("1", captor.getValue().getCode());
        assertEquals("一般医疗服务费", captor.getValue().getDisplay());
        assertEquals(9L, captor.getValue().getSortOrder());
        assertEquals("operator-1", captor.getValue().getCreatedBy());
    }

    @Test
    void addDictionaryShouldRejectDuplicateCode() {
        DictionaryAddDTO data = addDTO();
        when(dictionaryMapper.lockCodeSystem("900010")).thenReturn("900010");
        when(dictionaryMapper.countDuplicateCode("900010", "1", null)).thenReturn(1);

        DictionaryBusinessException exception = assertThrows(
                DictionaryBusinessException.class,
                () -> dictionaryService.addDictionary(data)
        );

        assertEquals("同一字典类型下字典编码不能重复", exception.getMessage());
        verify(dictionaryMapper, never()).insert(any(DictionaryConceptDO.class));
    }

    @Test
    void modifyDictionaryShouldKeepSortOrderWhenItIsOmitted() throws Exception {
        DictionaryModifyDTO data = new DictionaryModifyDTO();
        data.setId("900010001");
        copyAddFields(data);
        data.setSortOrder(null);
        mockOperator();
        when(dictionaryMapper.countConcept("900010001")).thenReturn(1);
        when(dictionaryMapper.lockCodeSystem("900010")).thenReturn("900010");
        when(dictionaryMapper.countDuplicateCode("900010", "1", "900010001")).thenReturn(0);
        when(dictionaryMapper.updateConcept(any(DictionaryConceptDO.class))).thenReturn(1);

        String id = dictionaryService.modifyDictionary(data);

        assertEquals("900010001", id);
        ArgumentCaptor<DictionaryConceptDO> captor = ArgumentCaptor.forClass(DictionaryConceptDO.class);
        verify(dictionaryMapper).updateConcept(captor.capture());
        assertNull(captor.getValue().getSortOrder());
    }

    @Test
    void removeDictionaryShouldRejectMissingOrDeletedConcept() throws Exception {
        DictionaryDeleteDTO data = new DictionaryDeleteDTO();
        data.setId("missing");
        mockOperator();
        when(dictionaryMapper.logicallyDelete("missing", "operator-1")).thenReturn(0);

        DictionaryBusinessException exception = assertThrows(
                DictionaryBusinessException.class,
                () -> dictionaryService.removeDictionary(data)
        );

        assertEquals("字典项不存在或已删除", exception.getMessage());
    }

    private DictionaryAddDTO addDTO() {
        DictionaryAddDTO data = new DictionaryAddDTO();
        copyAddFields(data);
        data.setSortOrder(1L);
        return data;
    }

    private void copyAddFields(DictionaryAddDTO data) {
        data.setCodeSystemId(" 900010 ");
        data.setCode(" 1 ");
        data.setDisplay(" 一般医疗服务费 ");
        data.setDefinition(" 一般医疗服务费用 ");
    }

    private void mockOperator() throws Exception {
        when(userHolder.getCurrentUser()).thenReturn(UserDTO.builder().id("operator-1").build());
    }
}
