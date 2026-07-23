package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeWithdrawApprovalDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;
import org.springframework.test.util.ReflectionTestUtils;

import java.math.BigDecimal;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.isNull;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

class StocktakeApprovalServiceImplTest {
    private StocktakeApprovalServiceImpl service;
    private WkfSupplyRequestMapper supplyRequestMapper;
    private WkfInventoryItemMapper inventoryItemMapper;

    @BeforeEach
    void setUp() {
        service = new StocktakeApprovalServiceImpl();
        supplyRequestMapper = mock(WkfSupplyRequestMapper.class);
        inventoryItemMapper = mock(WkfInventoryItemMapper.class);
        ReflectionTestUtils.setField(service, "supplyRequestMapper", supplyRequestMapper);
        ReflectionTestUtils.setField(service, "inventoryItemMapper", inventoryItemMapper);
    }

    @Test
    void submitDraftStocktakeUpdatesToPendingApproval() {
        when(supplyRequestMapper.selectList(any(QueryWrapper.class)))
                .thenReturn(List.of(stocktakeRow("DRAFT")));
        when(supplyRequestMapper.update(isNull(), any(UpdateWrapper.class))).thenReturn(1);

        assertTrue(service.submit(submitDto("CST2026-001")));

        verify(supplyRequestMapper).update(isNull(), any(UpdateWrapper.class));
        verify(inventoryItemMapper, never()).update(isNull(), any(UpdateWrapper.class));
    }

    @Test
    void withdrawPendingStocktakeUpdatesToWithdraw() {
        when(supplyRequestMapper.selectList(any(QueryWrapper.class)))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));
        when(supplyRequestMapper.update(isNull(), any(UpdateWrapper.class))).thenReturn(1);

        assertTrue(service.withdraw(withdrawDto("CST2026-001")));

        verify(supplyRequestMapper).update(isNull(), any(UpdateWrapper.class));
    }

    @Test
    void agreePendingStocktakeAdjustsInventoryAndApproves() {
        when(supplyRequestMapper.selectList(any(QueryWrapper.class)))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));
        when(inventoryItemMapper.update(isNull(), any(UpdateWrapper.class))).thenReturn(1);
        when(supplyRequestMapper.update(isNull(), any(UpdateWrapper.class))).thenReturn(1);

        assertTrue(service.agree(agreeDto("CST2026-001")));

        verify(inventoryItemMapper).update(isNull(), any(UpdateWrapper.class));
        verify(supplyRequestMapper).update(isNull(), any(UpdateWrapper.class));
    }

    @Test
    void agreeRequiresAgreeStatus() {
        StocktakeAgreeApprovalDTO dto = agreeDto("CST2026-001");
        dto.setStatus("PENDING_APPROVAL");

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class, () -> service.agree(dto));

        assertEquals("status must be AGREE when approving stocktake", ex.getMessage());
        verify(supplyRequestMapper, never()).selectList(any(QueryWrapper.class));
    }

    @Test
    void rejectPendingStocktakeRequiresRejectReason() {
        StocktakeRejectApprovalDTO dto = rejectDto("CST2026-001");

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class, () -> service.reject(dto));

        assertEquals("changeReason cannot be blank when rejecting approval", ex.getMessage());
        verify(supplyRequestMapper, never()).selectList(any(QueryWrapper.class));
    }

    @Test
    void rejectPendingStocktakeUpdatesToReject() {
        when(supplyRequestMapper.selectList(any(QueryWrapper.class)))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));
        when(supplyRequestMapper.update(isNull(), any(UpdateWrapper.class))).thenReturn(1);

        StocktakeRejectApprovalDTO dto = rejectDto("CST2026-001");
        dto.setChangeReasonCode("DIFF");
        dto.setChangeReason("inventory difference needs recheck");

        assertTrue(service.reject(dto));

        verify(supplyRequestMapper).update(isNull(), any(UpdateWrapper.class));
    }

    @Test
    void submitFailsWhenOrderDoesNotExist() {
        when(supplyRequestMapper.selectList(any(QueryWrapper.class))).thenReturn(List.of());

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class,
                () -> service.submit(submitDto("CST2026-404")));

        assertEquals("warehouse stocktake order does not exist: CST2026-404", ex.getMessage());
    }

    @Test
    void withdrawFailsWhenCurrentStatusIsNotPending() {
        when(supplyRequestMapper.selectList(any(QueryWrapper.class)))
                .thenReturn(List.of(stocktakeRow("DRAFT")));

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.withdraw(withdrawDto("CST2026-001")));

        assertEquals("current status DRAFT does not allow this approval action", ex.getMessage());
    }

    @Test
    void submitQueryIsLimitedToWarehouseStocktakeType() {
        when(supplyRequestMapper.selectList(any(QueryWrapper.class)))
                .thenReturn(List.of(stocktakeRow("DRAFT")));
        when(supplyRequestMapper.update(isNull(), any(UpdateWrapper.class))).thenReturn(1);
        ArgumentCaptor<QueryWrapper<WkfSupplyRequest>> captor = ArgumentCaptor.forClass(QueryWrapper.class);

        service.submit(submitDto("CST2026-001"));

        verify(supplyRequestMapper).selectList(captor.capture());
        String sqlSegment = captor.getValue().getSqlSegment();
        assertTrue(sqlSegment.contains("type_enum"));
        assertTrue(sqlSegment.contains("bus_no"));
    }

    private StocktakeSubmitApprovalDTO submitDto(String busNo) {
        StocktakeSubmitApprovalDTO dto = new StocktakeSubmitApprovalDTO();
        dto.setBusNo(busNo);
        return dto;
    }

    private StocktakeWithdrawApprovalDTO withdrawDto(String busNo) {
        StocktakeWithdrawApprovalDTO dto = new StocktakeWithdrawApprovalDTO();
        dto.setBusNo(busNo);
        return dto;
    }

    private StocktakeAgreeApprovalDTO agreeDto(String busNo) {
        StocktakeAgreeApprovalDTO dto = new StocktakeAgreeApprovalDTO();
        dto.setBusNo(busNo);
        dto.setStatus("AGREE");
        return dto;
    }

    private StocktakeRejectApprovalDTO rejectDto(String busNo) {
        StocktakeRejectApprovalDTO dto = new StocktakeRejectApprovalDTO();
        dto.setBusNo(busNo);
        return dto;
    }

    private WkfSupplyRequest stocktakeRow(String status) {
        WkfSupplyRequest row = new WkfSupplyRequest();
        row.setId("row-1");
        row.setBusNo("CST2026-001");
        row.setTypeEnum("CABINET_STOCKTAKE");
        row.setStatusEnum(status);
        row.setItemTable("med_medication_definition");
        row.setItemId("drug-1");
        row.setPurposeLocationId("warehouse-1");
        row.setLotNumber("LOT-1");
        row.setUnitCode("box");
        row.setTotalQuantity(BigDecimal.TEN);
        row.setIsDeleted(0);
        return row;
    }
}
