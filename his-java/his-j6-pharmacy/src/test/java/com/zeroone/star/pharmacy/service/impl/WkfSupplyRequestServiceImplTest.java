package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptSaveResultVO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.test.util.ReflectionTestUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.when;

class WkfSupplyRequestServiceImplTest {

    private TestableWkfSupplyRequestServiceImpl service;
    private WkfSupplyRequestMapper mapper;
    private UserHolder userHolder;

    @BeforeEach
    void setUp() throws Exception {
        service = new TestableWkfSupplyRequestServiceImpl();
        mapper = mock(WkfSupplyRequestMapper.class);
        userHolder = mock(UserHolder.class);
        ReflectionTestUtils.setField(service, "baseMapper", mapper);
        ReflectionTestUtils.setField(service, "userHolder", userHolder);
        doReturn(UserDTO.builder().id("u1001").username("tester").build()).when(userHolder).getCurrentUser();
    }

    @Test
    void addInboundReceiptCreatesDraftRows() {
        when(mapper.countActiveRowsByBusNo(anyString())).thenReturn(0L);

        InboundReceiptSaveResultVO result = service.addInboundReceipt(sampleAddDto(null, null));

        assertNotNull(result.getBusNo());
        assertEquals("DRAFT", result.getStatusEnum());
        assertEquals(0, result.getVersion());
        assertEquals("新增成功", result.getActionResult());
        assertEquals(1, service.savedRows.size());
        assertEquals(result.getBusNo(), service.savedRows.get(0).getBusNo());
        assertEquals("u1001", service.savedRows.get(0).getCreatedBy());
        assertEquals("u1001", service.savedRows.get(0).getUpdatedBy());
        assertFalse(service.savedRows.get(0).getIsDeleted());
    }

    @Test
    void saveInboundReceiptRebuildsRowsWithIncrementedVersion() {
        when(mapper.selectActiveRowsByBusNo("IR202607310001")).thenReturn(List.of(activeRow("IR202607310001", "DRAFT", 1)));
        when(mapper.updateActiveRowsDeletedByBusNo(anyString(), any(), anyString(), any(), anyString())).thenReturn(1);

        InboundReceiptSaveResultVO result = service.saveInboundReceipt(sampleAddDto("IR202607310001", 1));

        assertEquals("IR202607310001", result.getBusNo());
        assertEquals("DRAFT", result.getStatusEnum());
        assertEquals(2, result.getVersion());
        assertEquals("保存成功", result.getActionResult());
        assertEquals(1, service.savedRows.size());
        assertEquals(2, service.savedRows.get(0).getVersion());
        assertEquals("IR202607310001", service.savedRows.get(0).getBusNo());
    }

    @Test
    void saveAndSubmitInboundReceiptCreatesPendingApprovalRows() {
        when(mapper.selectActiveRowsByBusNo(anyString())).thenReturn(List.of());

        InboundReceiptSaveResultVO result = service.saveAndSubmitInboundReceipt(sampleAddDto(null, null));

        assertEquals("PENDING_APPROVAL", result.getStatusEnum());
        assertEquals("保存并提交审批成功", result.getActionResult());
        assertNotNull(result.getApplyTime());
        assertEquals(1, service.savedRows.size());
        assertEquals("PENDING_APPROVAL", service.savedRows.get(0).getStatusEnum());
        assertNotNull(service.savedRows.get(0).getApplyTime());
    }

    @Test
    void saveInboundReceiptRejectsEmptyLines() {
        InboundReceiptSaveDTO dto = new InboundReceiptSaveDTO();
        dto.setBusNo("IR202607310001");
        dto.setVersion(1);
        dto.setLines(new ArrayList<>());

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class, () -> service.saveInboundReceipt(dto));
        assertEquals("药库进货单明细不能为空", ex.getMessage());
    }

    private InboundReceiptSaveDTO sampleAddDto(String busNo, Integer version) {
        InboundReceiptSaveDTO dto = new InboundReceiptSaveDTO();
        dto.setBusNo(busNo);
        dto.setVersion(version);
        dto.setTypeEnum("CABINET_INBOUND_RECEIPT");
        dto.setCategoryEnum("STOCK_SUPPLY");
        dto.setPurposeTypeEnum("WAREHOUSE");
        dto.setPurposeLocationId("warehouse-1");
        dto.setSupplierId("supplier-1");
        dto.setLines(List.of(sampleLine()));
        return dto;
    }

    private InboundReceiptSaveDTO.InboundReceiptLineDTO sampleLine() {
        InboundReceiptSaveDTO.InboundReceiptLineDTO line = new InboundReceiptSaveDTO.InboundReceiptLineDTO();
        line.setItemTable("med_medication_definition");
        line.setItemId("drug-1");
        line.setItemQuantity(BigDecimal.ONE);
        line.setUnitCode("box");
        line.setOccurrenceTime(LocalDateTime.of(2026, 7, 31, 10, 0));
        line.setStartTime(LocalDateTime.of(2026, 7, 1, 0, 0));
        line.setEndTime(LocalDateTime.of(2028, 7, 1, 0, 0));
        line.setLotNumber("LOT-1");
        line.setTraceNo("TRACE-1");
        line.setPrice(new BigDecimal("36.50"));
        line.setTotalPrice(new BigDecimal("36.50"));
        line.setRemake("demo");
        return line;
    }

    private WkfSupplyRequest activeRow(String busNo, String status, Integer version) {
        WkfSupplyRequest row = new WkfSupplyRequest();
        row.setId("row-1");
        row.setBusNo(busNo);
        row.setStatusEnum(status);
        row.setVersion(version);
        row.setIsDeleted(false);
        return row;
    }

    private static class TestableWkfSupplyRequestServiceImpl extends WkfSupplyRequestServiceImpl {
        private final List<WkfSupplyRequest> savedRows = new ArrayList<>();

        @Override
        public boolean saveBatch(Collection<WkfSupplyRequest> entityList) {
            savedRows.clear();
            savedRows.addAll(entityList);
            return true;
        }
    }
}
