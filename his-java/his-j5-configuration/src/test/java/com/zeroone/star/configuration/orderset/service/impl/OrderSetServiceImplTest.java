package com.zeroone.star.configuration.orderset.service.impl;

import com.baomidou.mybatisplus.core.conditions.Wrapper;
import com.zeroone.star.configuration.orderset.domain.OrderSetDO;
import com.zeroone.star.configuration.orderset.domain.OrderSetLineDO;
import com.zeroone.star.configuration.orderset.mapper.OrderSetLineMapper;
import com.zeroone.star.configuration.orderset.mapper.OrderSetMapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetLineDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetStatusDTO;
import com.zeroone.star.project.exception.BusinessException;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyList;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
@SuppressWarnings({"unchecked", "rawtypes"})
class OrderSetServiceImplTest {

    @Mock
    private OrderSetMapper orderSetMapper;
    @Mock
    private OrderSetLineMapper orderSetLineMapper;
    @Mock
    private UserHolder userHolder;

    private OrderSetServiceImpl service;

    @BeforeEach
    void setUp() throws Exception {
        service = new OrderSetServiceImpl(orderSetMapper, orderSetLineMapper, userHolder);
        when(userHolder.getCurrentUser()).thenReturn(UserDTO.builder()
                .id("user-1")
                .tenantId("tenant-1")
                .build());
    }

    @Test
    void addShouldPublishWhenAtLeastOneValidLineExists() {
        OrderSetAddDTO dto = newAddDTO();
        dto.setLines(List.of(newMedicationLine(null, 1)));
        when(orderSetMapper.insert(any(OrderSetDO.class))).thenReturn(1);
        when(orderSetLineMapper.insertBatch(anyList())).thenAnswer(invocation ->
                ((List<?>) invocation.getArgument(0)).size());
        when(orderSetMapper.updateStatusByVersion(anyString(), eq("tenant-1"),
                eq("ACTIVE"), eq("user-1"), eq(0))).thenReturn(1);

        OrderSetOperationVO result = service.addOrderSet(dto);

        assertEquals("ACTIVE", result.getStatus());
        assertTrue(result.getPublished());
        assertEquals(1, result.getVersion());
    }

    @Test
    void addShouldKeepDraftWhenLinesAreEmpty() {
        OrderSetAddDTO dto = newAddDTO();
        when(orderSetMapper.insert(any(OrderSetDO.class))).thenReturn(1);

        OrderSetOperationVO result = service.addOrderSet(dto);

        assertEquals("DRAFT", result.getStatus());
        assertFalse(result.getPublished());
        assertTrue(result.getMessage().contains("空壳组套不可发布"));
        verify(orderSetMapper, never()).updateStatusByVersion(anyString(), anyString(),
                anyString(), anyString(), anyInt());
    }

    @Test
    void modifyShouldRejectRetiredOrderSet() {
        OrderSetDO existing = existingOrderSet("RETIRED", 2);
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(existing);
        OrderSetModifyDTO dto = newModifyDTO(2);

        BusinessException exception = assertThrows(BusinessException.class,
                () -> service.modifyOrderSet(dto));

        assertTrue(exception.getMessage().contains("不可修改"));
        verify(orderSetMapper, never()).updateContentByVersion(any(), anyInt());
    }

    @Test
    void modifyShouldSynchronizeNewChangedAndRemovedLines() {
        OrderSetDO existing = existingOrderSet("ACTIVE", 3);
        OrderSetLineDO changed = existingLine("line-1", 4);
        OrderSetLineDO removed = existingLine("line-2", 1);
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(existing);
        when(orderSetLineMapper.selectList(any(Wrapper.class))).thenReturn(List.of(changed, removed));
        when(orderSetMapper.updateContentByVersion(any(), eq(3))).thenReturn(1);
        when(orderSetLineMapper.insertBatch(anyList())).thenReturn(1);
        when(orderSetLineMapper.updateContentByVersion(any(), eq(4))).thenReturn(1);
        when(orderSetLineMapper.softDeleteByIds(anyList(), eq("tenant-1"),
                eq("order-set-1"), eq("user-1"))).thenReturn(1);

        OrderSetModifyDTO dto = newModifyDTO(3);
        dto.setLines(List.of(newMedicationLine("line-1", 1), newMedicationLine(null, 2)));

        OrderSetOperationVO result = service.modifyOrderSet(dto);

        assertEquals(4, result.getVersion());
        ArgumentCaptor<List<String>> removedIds = ArgumentCaptor.forClass(List.class);
        verify(orderSetLineMapper).softDeleteByIds(removedIds.capture(), eq("tenant-1"),
                eq("order-set-1"), eq("user-1"));
        assertEquals(List.of("line-2"), removedIds.getValue());
        verify(orderSetLineMapper).updateContentByVersion(changed, 4);
    }

    @Test
    void removeShouldSoftDeleteRootAndAllChildren() {
        OrderSetDO existing = existingOrderSet("ACTIVE", 5);
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(existing);
        when(orderSetMapper.softDeleteByVersion("order-set-1", "tenant-1", "user-1", 5))
                .thenReturn(1);

        assertEquals("order-set-1", service.removeOrderSet("order-set-1"));

        verify(orderSetLineMapper).softDeleteByTemplateId("tenant-1", "order-set-1", "user-1");
    }

    @Test
    void publishShouldRejectEmptyDraft() {
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(existingOrderSet("DRAFT", 0));
        when(orderSetLineMapper.selectCount(any(Wrapper.class))).thenReturn(0L);

        BusinessException exception = assertThrows(BusinessException.class,
                () -> service.modifyOrderSetStatus(statusDTO("PUBLISH", 0)));

        assertEquals("空壳组套不可发布", exception.getMessage());
    }

    @Test
    void publishShouldActivateNonEmptyDraft() {
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(existingOrderSet("DRAFT", 0));
        when(orderSetLineMapper.selectCount(any(Wrapper.class))).thenReturn(1L);
        when(orderSetMapper.updateStatusByVersion("order-set-1", "tenant-1",
                "ACTIVE", "user-1", 0)).thenReturn(1);

        OrderSetOperationVO result = service.modifyOrderSetStatus(statusDTO("ENABLE", 0));

        assertEquals("ACTIVE", result.getStatus());
        assertTrue(result.getPublished());
        assertEquals(1, result.getVersion());
    }

    @Test
    void retireShouldChangeActiveToRetired() {
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(existingOrderSet("ACTIVE", 7));
        when(orderSetMapper.updateStatusByVersion("order-set-1", "tenant-1",
                "RETIRED", "user-1", 7)).thenReturn(1);

        OrderSetOperationVO result = service.modifyOrderSetStatus(statusDTO("RETIRE", 7));

        assertEquals("RETIRED", result.getStatus());
        assertFalse(result.getPublished());
        assertEquals(8, result.getVersion());
    }

    @Test
    void publishShouldRejectRetiredOrderSet() {
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(existingOrderSet("RETIRED", 8));

        BusinessException exception = assertThrows(BusinessException.class,
                () -> service.modifyOrderSetStatus(statusDTO("PUBLISH", 8)));

        assertTrue(exception.getMessage().contains("不可重新发布"));
    }

    @Test
    void modifyShouldRejectStaleVersion() {
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(existingOrderSet("ACTIVE", 5));

        BusinessException exception = assertThrows(BusinessException.class,
                () -> service.modifyOrderSet(newModifyDTO(4)));

        assertTrue(exception.getMessage().contains("刷新后重试"));
        verify(orderSetMapper, never()).updateContentByVersion(any(), anyInt());
    }

    @Test
    void removeShouldHideNotFoundAndCrossTenantCasesBehindSameMessage() {
        when(orderSetMapper.selectOne(any(Wrapper.class))).thenReturn(null);

        BusinessException exception = assertThrows(BusinessException.class,
                () -> service.removeOrderSet("other-tenant-order-set"));

        assertEquals("组套模板不存在或无权访问", exception.getMessage());
        verify(orderSetMapper, never()).softDeleteByVersion(anyString(), anyString(),
                anyString(), anyInt());
    }

    private OrderSetAddDTO newAddDTO() {
        OrderSetAddDTO dto = new OrderSetAddDTO();
        dto.setObjectId("department-1");
        dto.setBindingEnum("DEPARTMENT");
        dto.setSceneEnum("OUTPATIENT");
        dto.setName("普通感冒组套");
        dto.setTemplateKindEnum("GENERAL");
        dto.setLines(new ArrayList<>());
        return dto;
    }

    private OrderSetModifyDTO newModifyDTO(int version) {
        OrderSetModifyDTO dto = new OrderSetModifyDTO();
        dto.setId("order-set-1");
        dto.setVersion(version);
        dto.setObjectId("department-1");
        dto.setBindingEnum("DEPARTMENT");
        dto.setSceneEnum("OUTPATIENT");
        dto.setName("修改后的组套");
        dto.setTemplateKindEnum("GENERAL");
        dto.setLines(new ArrayList<>());
        return dto;
    }

    private OrderSetLineDTO newMedicationLine(String id, int sortOrder) {
        OrderSetLineDTO dto = new OrderSetLineDTO();
        dto.setId(id);
        dto.setLineKindEnum("MEDICATION");
        dto.setMedicationDefinitionId("medication-" + sortOrder);
        dto.setSortOrder(sortOrder);
        return dto;
    }

    private OrderSetDO existingOrderSet(String status, int version) {
        OrderSetDO entity = new OrderSetDO();
        entity.setId("order-set-1");
        entity.setTenantId("tenant-1");
        entity.setObjectId("department-1");
        entity.setBindingEnum("DEPARTMENT");
        entity.setSceneEnum("OUTPATIENT");
        entity.setTemplateKindEnum("GENERAL");
        entity.setStatusEnum(status);
        entity.setVersion(version);
        return entity;
    }

    private OrderSetLineDO existingLine(String id, int version) {
        OrderSetLineDO entity = new OrderSetLineDO();
        entity.setId(id);
        entity.setTenantId("tenant-1");
        entity.setOrderTemplateId("order-set-1");
        entity.setVersion(version);
        return entity;
    }

    private OrderSetStatusDTO statusDTO(String operation, int version) {
        OrderSetStatusDTO dto = new OrderSetStatusDTO();
        dto.setId("order-set-1");
        dto.setVersion(version);
        dto.setOperationType(operation);
        return dto;
    }
}
