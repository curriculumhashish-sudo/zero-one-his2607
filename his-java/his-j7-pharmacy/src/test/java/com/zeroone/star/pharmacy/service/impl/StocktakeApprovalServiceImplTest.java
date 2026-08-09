package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.DocInventoryItemSnapshotMapper;
import com.zeroone.star.pharmacy.mapper.InventoryLockMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * G8 盘点单审批接口测试。
 *
 * @author 北海
 * @version 1.0.0
 */
@ExtendWith(MockitoExtension.class)
class StocktakeApprovalServiceImplTest {

    private static final String BUS_NO = "PST202608080001";
    private static final String APPROVER_ID = "approver-001";

    @Mock
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Mock
    private WkfInventoryItemMapper wkfInventoryItemMapper;

    @Mock
    private InventoryLockMapper inventoryLockMapper;

    @Mock
    private DocInventoryItemSnapshotMapper docInventoryItemSnapshotMapper;

    @InjectMocks
    private WkfSupplyRequestServiceImpl service;

    private WkfSupplyRequest pendingRequest;

    @BeforeEach
    void setUp() {
        pendingRequest = new WkfSupplyRequest();
        pendingRequest.setBusNo(BUS_NO);
        pendingRequest.setTypeEnum("PHARMACY_STOCKTAKE");
        pendingRequest.setStatusEnum("PENDING_APPROVAL");
        pendingRequest.setCreatedBy("creator-001");
        pendingRequest.setCreatedAt(LocalDateTime.of(2026, 8, 8, 9, 0));
        pendingRequest.setApplicantId("applicant-001");
        pendingRequest.setApplyTime(LocalDateTime.of(2026, 8, 8, 9, 30));
        pendingRequest.setItemQuantity(BigDecimal.ZERO);
    }

    @Test
    void approveUsesApprovalTimeFromRequest() {
        LocalDateTime approvalTime = LocalDateTime.of(2026, 8, 8, 10, 0);
        ApproveDTO dto = buildApproveDTO(approvalTime);
        when(wkfSupplyRequestMapper.getPharmacyStockTakeList(BUS_NO)).thenReturn(List.of(pendingRequest));

        var result = service.approve(dto);

        assertEquals("AGREE", result.getStatusEnum());
        assertEquals(approvalTime, result.getApprovalTime());
        verify(wkfSupplyRequestMapper).updateStatusForApprove("AGREE", APPROVER_ID, approvalTime, BUS_NO);
        verify(inventoryLockMapper).deleteByBusNo(BUS_NO);
    }

    @Test
    void rejectUsesApprovalTimeFromRequest() {
        LocalDateTime approvalTime = LocalDateTime.of(2026, 8, 8, 10, 15);
        ApproveDTO dto = buildApproveDTO(approvalTime);
        when(wkfSupplyRequestMapper.getPharmacyStockTakeList(BUS_NO)).thenReturn(List.of(pendingRequest));

        var result = service.reject(dto);

        assertEquals("REJECT", result.getStatusEnum());
        assertEquals(approvalTime, result.getApprovalTime());
        verify(wkfSupplyRequestMapper).updateStatusForApprove("REJECT", APPROVER_ID, approvalTime, BUS_NO);
        verify(inventoryLockMapper).deleteByBusNo(BUS_NO);
    }

    private ApproveDTO buildApproveDTO(LocalDateTime approvalTime) {
        ApproveDTO dto = new ApproveDTO();
        dto.setBusNo(BUS_NO);
        dto.setApproverId(APPROVER_ID);
        dto.setApprovalTime(approvalTime);
        return dto;
    }
}
