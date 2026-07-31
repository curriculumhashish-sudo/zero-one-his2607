package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.Wrapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.InboundApprovalMapper;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j7.pharmacy.InboundApprovalDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.isNull;
import static org.mockito.Mockito.lenient;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * G7 入库单审批流转服务测试。
 *
 * @author lvv
 * @version 1.0.0
 */
@ExtendWith(MockitoExtension.class)
class InboundApprovalServiceImplTest {

    private static final String BUS_NO = "CIR2026-06-300001";

    @Mock
    private InboundApprovalMapper inboundApprovalMapper;

    @Mock
    private UserHolder userHolder;

    @InjectMocks
    private InboundApprovalServiceImpl service;

    private WkfSupplyRequest pendingOrder;

    @BeforeEach
    void setUp() throws Exception {
        pendingOrder = new WkfSupplyRequest();
        pendingOrder.setBusNo(BUS_NO);
        pendingOrder.setTypeEnum("CABINET_INBOUND_RECEIPT");
        pendingOrder.setStatusEnum("PENDING_APPROVAL");
        pendingOrder.setApplicantId("user-001");

        UserDTO operator = UserDTO.builder()
                .id("user-001")
                .username("lvv")
                .build();
        // lvv：部分前置校验场景不会读取登录用户，使用宽松桩避免无关测试失败。
        lenient().when(userHolder.getCurrentUser()).thenReturn(operator);
    }

    @Test
    void withdrawUpdatesAllDetailsAndNormalizesInput() {
        mockPendingOrderAndUpdatedRows(2);
        InboundApprovalDTO dto = new InboundApprovalDTO("  " + BUS_NO + "  ", "  填写有误  ");

        var result = service.withdrawApproval(dto);

        assertEquals(BUS_NO, dto.getBusNo());
        assertEquals("填写有误", dto.getRemark());
        assertEquals("WITHDRAW", result.getStatus());
        verify(inboundApprovalMapper).update(isNull(), any(Wrapper.class));
    }

    @Test
    void approveUpdatesAllDetails() {
        mockPendingOrderAndUpdatedRows(2);

        var result = service.approveApproval(new InboundApprovalDTO(BUS_NO, "同意入库"));

        assertEquals(BUS_NO, result.getBusNo());
        assertEquals("AGREE", result.getStatus());
        assertEquals("审批通过", result.getMessage());
    }

    @Test
    void rejectRequiresReasonAndUpdatesAllDetails() {
        IllegalArgumentException blankReason = assertThrows(
                IllegalArgumentException.class,
                () -> service.rejectApproval(new InboundApprovalDTO(BUS_NO, "   "))
        );
        assertEquals("驳回原因不能为空", blankReason.getMessage());
        verify(inboundApprovalMapper, never()).selectOne(any(Wrapper.class));

        mockPendingOrderAndUpdatedRows(1);
        var result = service.rejectApproval(new InboundApprovalDTO(BUS_NO, "数量不符"));
        assertEquals("REJECT", result.getStatus());
    }

    @Test
    void operationRejectsMissingOrder() {
        when(inboundApprovalMapper.selectOne(any(Wrapper.class))).thenReturn(null);

        IllegalArgumentException error = assertThrows(
                IllegalArgumentException.class,
                () -> service.approveApproval(new InboundApprovalDTO(BUS_NO, null))
        );

        assertEquals("入库单不存在：" + BUS_NO, error.getMessage());
        verify(inboundApprovalMapper, never()).update(isNull(), any(Wrapper.class));
    }

    @Test
    void operationRejectsOrderOutsidePendingStatus() {
        pendingOrder.setStatusEnum("AGREE");
        when(inboundApprovalMapper.selectOne(any(Wrapper.class))).thenReturn(pendingOrder);

        IllegalArgumentException error = assertThrows(
                IllegalArgumentException.class,
                () -> service.approveApproval(new InboundApprovalDTO(BUS_NO, null))
        );

        assertEquals("入库单不在待审批状态，当前状态：AGREE", error.getMessage());
    }

    @Test
    void withdrawCanOnlyBePerformedByApplicant() throws Exception {
        when(inboundApprovalMapper.selectOne(any(Wrapper.class))).thenReturn(pendingOrder);
        when(userHolder.getCurrentUser()).thenReturn(UserDTO.builder()
                .id("other-user")
                .username("approver")
                .build());

        IllegalArgumentException error = assertThrows(
                IllegalArgumentException.class,
                () -> service.withdrawApproval(new InboundApprovalDTO(BUS_NO, "撤回"))
        );

        assertEquals("只能由申请人撤回该入库单：" + BUS_NO, error.getMessage());
        verify(inboundApprovalMapper, never()).update(isNull(), any(Wrapper.class));
    }

    @Test
    void operationDetectsConcurrentStatusChange() {
        mockPendingOrderAndUpdatedRows(0);

        IllegalArgumentException error = assertThrows(
                IllegalArgumentException.class,
                () -> service.approveApproval(new InboundApprovalDTO(BUS_NO, null))
        );

        assertEquals("审批流转失败，单据可能已被其他操作处理：" + BUS_NO, error.getMessage());
    }

    private void mockPendingOrderAndUpdatedRows(int rows) {
        when(inboundApprovalMapper.selectOne(any(Wrapper.class))).thenReturn(pendingOrder);
        when(inboundApprovalMapper.update(isNull(), any(Wrapper.class))).thenReturn(rows);
    }
}
