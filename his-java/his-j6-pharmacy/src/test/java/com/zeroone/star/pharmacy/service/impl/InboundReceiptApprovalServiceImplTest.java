package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.RejectInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.SubmitInboundReceiptApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.WithdrawInboundReceiptApprovalDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.test.util.ReflectionTestUtils;

import java.math.BigDecimal;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

class InboundReceiptApprovalServiceImplTest {

    private InboundReceiptApprovalServiceImpl service;
    private WkfSupplyRequestMapper supplyRequestMapper;
    private WkfInventoryItemMapper inventoryItemMapper;
    private MedMedicationDefinitionMapper medicationDefinitionMapper;

    @BeforeEach
    void setUp() {
        service = new InboundReceiptApprovalServiceImpl();
        supplyRequestMapper = mock(WkfSupplyRequestMapper.class);
        inventoryItemMapper = mock(WkfInventoryItemMapper.class);
        medicationDefinitionMapper = mock(MedMedicationDefinitionMapper.class);
        ReflectionTestUtils.setField(service, "supplyRequestMapper", supplyRequestMapper);
        ReflectionTestUtils.setField(service, "inventoryItemMapper", inventoryItemMapper);
        ReflectionTestUtils.setField(service, "medicationDefinitionMapper", medicationDefinitionMapper);
    }

    // ==================== submit ====================

    @Test
    void submitDraftInboundUpdatesToPendingApproval() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("DRAFT")));
        when(supplyRequestMapper.updateInboundToPending(anyString(), anyString(), anyString(),
                any(), any(), any(), any(), anyString())).thenReturn(1);

        assertTrue(service.submit(submitDto("IR2026-001")).getSuccess());
        verify(supplyRequestMapper).updateInboundToPending(
                eq("IR2026-001"), eq("CABINET_INBOUND_RECEIPT"), eq("DRAFT"),
                any(), any(), any(), any(), anyString());
        verify(inventoryItemMapper, never()).addQuantity(any(), any(), any(), any(), any(), any(), any(), any(), any(), any());
    }

    @Test
    void submitWithdrawnInboundCanBeSubmittedAgain() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("WITHDRAW")));
        when(supplyRequestMapper.updateInboundToPending(anyString(), anyString(), anyString(),
                any(), any(), any(), any(), anyString())).thenReturn(1);

        assertTrue(service.submit(submitDto("IR2026-001")).getSuccess());
    }

    @Test
    void submitRejectedInboundCanBeSubmittedAgain() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("REJECT")));
        when(supplyRequestMapper.updateInboundToPending(anyString(), anyString(), anyString(),
                any(), any(), any(), any(), anyString())).thenReturn(1);

        assertTrue(service.submit(submitDto("IR2026-001")).getSuccess());
    }

    @Test
    void submitFailsWhenAlreadyPendingApproval() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("PENDING_APPROVAL")));

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.submit(submitDto("IR2026-001")));
        assertEquals("当前状态 PENDING_APPROVAL 不允许执行此操作", ex.getMessage());
        verify(supplyRequestMapper, never()).updateInboundToPending(
                anyString(), anyString(), anyString(), any(), any(), any(), any(), anyString());
    }

    @Test
    void submitReturnsPendingApprovalStatus() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("DRAFT")));
        when(supplyRequestMapper.updateInboundToPending(anyString(), anyString(), anyString(),
                any(), any(), any(), any(), anyString())).thenReturn(1);

        assertEquals("PENDING_APPROVAL", service.submit(submitDto("IR2026-001")).getStatusEnum());
    }

    // ==================== withdraw ====================

    @Test
    void withdrawPendingInboundUpdatesToWithdraw() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("PENDING_APPROVAL")));
        when(supplyRequestMapper.updateInboundToWithdraw(anyString(), anyString(), anyString(),
                any(), anyString())).thenReturn(1);

        assertTrue(service.withdraw(withdrawDto("IR2026-001")).getSuccess());
        assertEquals("WITHDRAW", service.withdraw(withdrawDto("IR2026-001")).getStatusEnum());
    }

    @Test
    void withdrawFailsWhenCurrentStatusIsDraft() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("DRAFT")));

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.withdraw(withdrawDto("IR2026-001")));
        assertEquals("当前状态 DRAFT 不允许执行此操作", ex.getMessage());
    }

    // ==================== agree ====================

    @Test
    void agreePendingInboundAdjustsInventoryAndApproves() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("PENDING_APPROVAL")));
        when(inventoryItemMapper.addQuantity(any(), any(), any(), any(), any(), any(), any(), any(), any(), any()))
                .thenReturn(1); // 已有库存行，累加数量
        when(supplyRequestMapper.updateInboundToAgree(anyString(), anyString(), anyString(),
                any(), any(), any(), anyString())).thenReturn(1);

        assertTrue(service.agree(agreeDto("IR2026-001")).getSuccess());

        verify(inventoryItemMapper).addQuantity(
                eq("med_medication_definition"), eq("drug-1"), eq("warehouse-1"),
                eq("LOT-1"), eq("box"), eq(BigDecimal.TEN), any(), any(), any(), anyString());
        verify(supplyRequestMapper).updateInboundToAgree(
                eq("IR2026-001"), eq("CABINET_INBOUND_RECEIPT"), eq("PENDING_APPROVAL"),
                any(), any(), any(), anyString());
        assertEquals("AGREE", service.agree(agreeDto("IR2026-001")).getStatusEnum());
    }

    @Test
    void agreeFailsWhenOrderDoesNotExist() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of());

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class,
                () -> service.agree(agreeDto("IR2026-404")));
        assertEquals("进货单不存在: IR2026-404", ex.getMessage());
        verify(inventoryItemMapper, never()).addQuantity(any(), any(), any(), any(), any(), any(), any(), any(), any(), any());
    }

    @Test
    void agreeFailsWhenStatusUpdateAffectsNoRows() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("PENDING_APPROVAL")));
        when(inventoryItemMapper.addQuantity(any(), any(), any(), any(), any(), any(), any(), any(), any(), any()))
                .thenReturn(1);
        when(supplyRequestMapper.updateInboundToAgree(anyString(), anyString(), anyString(),
                any(), any(), any(), anyString())).thenReturn(0);

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.agree(agreeDto("IR2026-001")));
        assertEquals("更新审批状态失败，可能已被并发修改", ex.getMessage());
    }

    @Test
    void agreeFailsWhenRowsHaveInconsistentStatuses() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("DRAFT"), inboundRow("PENDING_APPROVAL")));

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.agree(agreeDto("IR2026-001")));
        assertTrue(ex.getMessage().startsWith("进货单内各行状态不一致"));
    }

    // ==================== reject ====================

    @Test
    void rejectRequiresRejectReason() {
        RejectInboundReceiptDTO dto = rejectDto("IR2026-001");
        // rejectReason 为空

        IllegalArgumentException ex = assertThrows(IllegalArgumentException.class,
                () -> service.reject(dto));
        assertEquals("驳回原因不能为空", ex.getMessage());
        verify(supplyRequestMapper, never()).selectInboundRowsForApproval(anyString(), anyString());
    }

    @Test
    void rejectPendingInboundUpdatesToReject() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("PENDING_APPROVAL")));
        when(supplyRequestMapper.updateInboundToReject(anyString(), anyString(), anyString(),
                any(), any(), anyString(), any(), anyString())).thenReturn(1);

        RejectInboundReceiptDTO dto = rejectDto("IR2026-001");
        dto.setRejectReason("单据信息不完整");

        assertTrue(service.reject(dto).getSuccess());
        assertEquals("REJECT", service.reject(dto).getStatusEnum());
    }

    @Test
    void rejectFailsWhenCurrentStatusIsDraft() {
        when(supplyRequestMapper.selectInboundRowsForApproval(anyString(), anyString()))
                .thenReturn(List.of(inboundRow("DRAFT")));

        RejectInboundReceiptDTO dto = rejectDto("IR2026-001");
        dto.setRejectReason("单据信息不完整");

        IllegalStateException ex = assertThrows(IllegalStateException.class,
                () -> service.reject(dto));
        assertEquals("当前状态 DRAFT 不允许执行此操作", ex.getMessage());
    }

    // ==================== helpers ====================

    private SubmitInboundReceiptApprovalDTO submitDto(String busNo) {
        SubmitInboundReceiptApprovalDTO dto = new SubmitInboundReceiptApprovalDTO();
        dto.setBusNo(busNo);
        return dto;
    }

    private WithdrawInboundReceiptApprovalDTO withdrawDto(String busNo) {
        WithdrawInboundReceiptApprovalDTO dto = new WithdrawInboundReceiptApprovalDTO();
        dto.setBusNo(busNo);
        return dto;
    }

    private AgreeInboundReceiptDTO agreeDto(String busNo) {
        AgreeInboundReceiptDTO dto = new AgreeInboundReceiptDTO();
        dto.setBusNo(busNo);
        return dto;
    }

    private RejectInboundReceiptDTO rejectDto(String busNo) {
        RejectInboundReceiptDTO dto = new RejectInboundReceiptDTO();
        dto.setBusNo(busNo);
        return dto;
    }

    private WkfSupplyRequest inboundRow(String status) {
        WkfSupplyRequest row = new WkfSupplyRequest();
        row.setId("row-1");
        row.setBusNo("IR2026-001");
        row.setTypeEnum("CABINET_INBOUND_RECEIPT");
        row.setStatusEnum(status);
        row.setItemTable("med_medication_definition");
        row.setItemId("drug-1");
        row.setPurposeLocationId("warehouse-1");
        row.setLotNumber("LOT-1");
        row.setUnitCode("box");
        row.setItemQuantity(BigDecimal.TEN);
        row.setIsDeleted(false);
        return row;
    }
}
