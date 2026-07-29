package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeWithdrawApprovalDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.test.util.ReflectionTestUtils;

import java.math.BigDecimal;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
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
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("DRAFT")));
        when(supplyRequestMapper.updateStocktakeToPending(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString()))
                .thenReturn(1);

        assertTrue(service.submit(submitDto("CST2026-001")));

        verify(supplyRequestMapper).updateStocktakeToPending(
                eq("CST2026-001"), eq("CABINET_STOCKTAKE"), eq("DRAFT"), anyString(), any(), any(), anyString());
        verify(inventoryItemMapper, never()).updateQuantityForStocktake(
                any(), any(), any(), any(), any(), any(), any(), any());
    }

    @Test
    void submitWithdrawnStocktakeCanBeSubmittedAgain() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("WITHDRAW")));
        when(supplyRequestMapper.updateStocktakeToPending(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString()))
                .thenReturn(1);

        assertTrue(service.submit(submitDto("CST2026-001")));

        verify(supplyRequestMapper).updateStocktakeToPending(
                eq("CST2026-001"), eq("CABINET_STOCKTAKE"), eq("WITHDRAW"), anyString(), any(), any(), anyString());
    }

    @Test
    void submitRejectedStocktakeCanBeSubmittedAgain() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("REJECT")));
        when(supplyRequestMapper.updateStocktakeToPending(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString()))
                .thenReturn(1);

        assertTrue(service.submit(submitDto("CST2026-001")));

        verify(supplyRequestMapper).updateStocktakeToPending(
                eq("CST2026-001"), eq("CABINET_STOCKTAKE"), eq("REJECT"), anyString(), any(), any(), anyString());
    }

    @Test
    void submitFailsWhenAlreadyPendingApproval() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.submit(submitDto("CST2026-001")));

        assertEquals("current status PENDING_APPROVAL does not allow this approval action", ex.getMessage());
        verify(supplyRequestMapper, never()).updateStocktakeToPending(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString());
    }

    @Test
    void submitFailsWhenStatusUpdateAffectsNoRows() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("DRAFT")));
        when(supplyRequestMapper.updateStocktakeToPending(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString()))
                .thenReturn(0);

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.submit(submitDto("CST2026-001")));

        assertEquals("failed to update stocktake approval status", ex.getMessage());
    }

    @Test
    void withdrawPendingStocktakeUpdatesToWithdraw() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));
        when(supplyRequestMapper.updateStocktakeToWithdraw(anyString(), anyString(), anyString(), any(), anyString()))
                .thenReturn(1);

        assertTrue(service.withdraw(withdrawDto("CST2026-001")));

        verify(supplyRequestMapper).updateStocktakeToWithdraw(
                eq("CST2026-001"), eq("CABINET_STOCKTAKE"), eq("PENDING_APPROVAL"), any(), anyString());
    }

    @Test
    void agreePendingStocktakeAdjustsInventoryAndApproves() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));
        when(inventoryItemMapper.updateQuantityForStocktake(any(), any(), any(), any(), any(), any(), any(), any()))
                .thenReturn(1);
        when(supplyRequestMapper.updateStocktakeToAgree(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString()))
                .thenReturn(1);

        assertTrue(service.agree(agreeDto("CST2026-001")));

        verify(inventoryItemMapper).updateQuantityForStocktake(
                eq("med_medication_definition"), eq("drug-1"), eq("warehouse-1"), eq("LOT-1"), eq("box"),
                eq(BigDecimal.TEN), any(), anyString());
        verify(supplyRequestMapper).updateStocktakeToAgree(
                eq("CST2026-001"), eq("CABINET_STOCKTAKE"), eq("PENDING_APPROVAL"), anyString(), any(), any(), anyString());
    }

    @Test
    void agreeRequiresAgreeStatus() {
        StocktakeAgreeApprovalDTO dto = agreeDto("CST2026-001");
        dto.setStatus("PENDING_APPROVAL");

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class, () -> service.agree(dto));

        assertEquals("status must be AGREE when approving stocktake", ex.getMessage());
        verify(supplyRequestMapper, never()).selectStocktakeRowsForApproval(anyString(), anyString());
    }

    @Test
    void rejectPendingStocktakeRequiresRejectReason() {
        StocktakeRejectApprovalDTO dto = rejectDto("CST2026-001");

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class, () -> service.reject(dto));

        assertEquals("changeReason cannot be blank when rejecting approval", ex.getMessage());
        verify(supplyRequestMapper, never()).selectStocktakeRowsForApproval(anyString(), anyString());
    }

    @Test
    void rejectPendingStocktakeUpdatesToReject() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));
        when(supplyRequestMapper.updateStocktakeToReject(
                anyString(), anyString(), anyString(), anyString(), any(), anyString(), any(), anyString()))
                .thenReturn(1);

        StocktakeRejectApprovalDTO dto = rejectDto("CST2026-001");
        dto.setChangeReasonCode("DIFF");
        dto.setChangeReason("inventory difference needs recheck");

        assertTrue(service.reject(dto));

        verify(supplyRequestMapper).updateStocktakeToReject(
                eq("CST2026-001"), eq("CABINET_STOCKTAKE"), eq("PENDING_APPROVAL"), anyString(), any(),
                eq("DIFF:inventory difference needs recheck"), any(), anyString());
    }

    @Test
    void submitFailsWhenOrderDoesNotExist() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString())).thenReturn(List.of());

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class,
                () -> service.submit(submitDto("CST2026-404")));

        assertEquals("warehouse stocktake order does not exist: CST2026-404", ex.getMessage());
    }

    @Test
    void withdrawFailsWhenCurrentStatusIsNotPending() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("DRAFT")));

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.withdraw(withdrawDto("CST2026-001")));

        assertEquals("current status DRAFT does not allow this approval action", ex.getMessage());
    }

    @Test
    void submitQueryIsLimitedToWarehouseStocktakeType() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("DRAFT")));
        when(supplyRequestMapper.updateStocktakeToPending(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString()))
                .thenReturn(1);

        service.submit(submitDto("CST2026-001"));

        verify(supplyRequestMapper).selectStocktakeRowsForApproval("CST2026-001", "CABINET_STOCKTAKE");
    }

    @Test
    void submitFailsWhenRowsHaveDifferentStatuses() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("DRAFT"), stocktakeRow("PENDING_APPROVAL")));

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.submit(submitDto("CST2026-001")));

        assertTrue(ex.getMessage().startsWith("stocktake order has inconsistent row statuses"));
        verify(supplyRequestMapper, never()).updateStocktakeToPending(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString());
    }

    @Test
    void agreeFailsWhenInventoryItemDoesNotExist() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));
        when(inventoryItemMapper.updateQuantityForStocktake(any(), any(), any(), any(), any(), any(), any(), any()))
                .thenReturn(0);

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.agree(agreeDto("CST2026-001")));

        assertEquals("inventory item not found for stocktake row id: row-1", ex.getMessage());
        verify(supplyRequestMapper, never()).updateStocktakeToAgree(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString());
    }

    @Test
    void agreeFailsWhenActualQuantityIsNull() {
        WkfSupplyRequest row = stocktakeRow("PENDING_APPROVAL");
        row.setTotalQuantity(null);
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(row));

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.agree(agreeDto("CST2026-001")));

        assertEquals("stocktake actual quantity cannot be null, row id: row-1", ex.getMessage());
        verify(inventoryItemMapper, never()).updateQuantityForStocktake(
                any(), any(), any(), any(), any(), any(), any(), any());
        verify(supplyRequestMapper, never()).updateStocktakeToAgree(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString());
    }

    @Test
    void agreeFailsWhenStatusUpdateAffectsNoRowsAfterInventoryAdjusted() {
        when(supplyRequestMapper.selectStocktakeRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(stocktakeRow("PENDING_APPROVAL")));
        when(inventoryItemMapper.updateQuantityForStocktake(any(), any(), any(), any(), any(), any(), any(), any()))
                .thenReturn(1);
        when(supplyRequestMapper.updateStocktakeToAgree(
                anyString(), anyString(), anyString(), anyString(), any(), any(), anyString()))
                .thenReturn(0);

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.agree(agreeDto("CST2026-001")));

        assertEquals("failed to update stocktake approval status", ex.getMessage());
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
        row.setIsDeleted(false);
        return row;
    }
}
