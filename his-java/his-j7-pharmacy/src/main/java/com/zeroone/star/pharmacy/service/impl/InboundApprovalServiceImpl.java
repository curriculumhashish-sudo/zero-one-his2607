package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.InboundApprovalMapper;
import com.zeroone.star.pharmacy.service.IInboundApprovalService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j7.pharmacy.InboundApprovalDTO;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;

/**
 * G7 入库单审批流转服务实现。
 * <p>
 * 同一入库单号可能对应多条药品明细，本服务按 {@code bus_no} 批量流转，
 * 并在更新条件中限定原状态，防止重复审批或并发覆盖。
 * </p>
 *
 * @author lvv
 * @version 1.0.0
 */
@Slf4j
@Service
public class InboundApprovalServiceImpl implements IInboundApprovalService {

    private static final String TYPE_CABINET_INBOUND_RECEIPT = "CABINET_INBOUND_RECEIPT";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String STATUS_WITHDRAW = "WITHDRAW";
    private static final String STATUS_AGREE = "AGREE";
    private static final String STATUS_REJECT = "REJECT";

    @Resource
    private InboundApprovalMapper inboundApprovalMapper;

    @Resource
    private UserHolder userHolder;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalVO withdrawApproval(InboundApprovalDTO dto) {
        normalize(dto);
        requireRemark(dto, "撤回原因不能为空");
        WkfSupplyRequest record = requirePendingInboundOrder(dto.getBusNo());
        UserDTO operator = currentUser();

        // lvv：有登录用户信息时，只允许申请人撤回自己的入库单。
        if (operator != null && StringUtils.hasText(operator.getId())
                && StringUtils.hasText(record.getApplicantId())
                && !operator.getId().equals(record.getApplicantId())) {
            throw new IllegalArgumentException("只能由申请人撤回该入库单：" + dto.getBusNo());
        }

        int rows = updatePendingOrder(dto, STATUS_WITHDRAW, operator, false);
        ensureUpdated(rows, dto.getBusNo());
        log.info("lvv：入库单 {} 已撤回，更新明细 {} 行", dto.getBusNo(), rows);
        return new InboundApprovalVO(dto.getBusNo(), STATUS_WITHDRAW, "撤回审批成功");
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalVO approveApproval(InboundApprovalDTO dto) {
        normalize(dto);
        requirePendingInboundOrder(dto.getBusNo());
        UserDTO operator = currentUser();
        int rows = updatePendingOrder(dto, STATUS_AGREE, operator, true);
        ensureUpdated(rows, dto.getBusNo());
        log.info("lvv：入库单 {} 审批通过，更新明细 {} 行", dto.getBusNo(), rows);
        return new InboundApprovalVO(dto.getBusNo(), STATUS_AGREE, "审批通过");
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalVO rejectApproval(InboundApprovalDTO dto) {
        normalize(dto);
        requireRemark(dto, "驳回原因不能为空");
        requirePendingInboundOrder(dto.getBusNo());
        UserDTO operator = currentUser();
        int rows = updatePendingOrder(dto, STATUS_REJECT, operator, true);
        ensureUpdated(rows, dto.getBusNo());
        log.info("lvv：入库单 {} 已驳回，更新明细 {} 行", dto.getBusNo(), rows);
        return new InboundApprovalVO(dto.getBusNo(), STATUS_REJECT, "驳回审批成功");
    }

    /**
     * lvv：查询并校验入库单存在且仍处于待审批状态。
     */
    private WkfSupplyRequest requirePendingInboundOrder(String busNo) {
        LambdaQueryWrapper<WkfSupplyRequest> query = new LambdaQueryWrapper<>();
        query.eq(WkfSupplyRequest::getBusNo, busNo)
                .eq(WkfSupplyRequest::getTypeEnum, TYPE_CABINET_INBOUND_RECEIPT)
                .last("LIMIT 1");
        WkfSupplyRequest record = inboundApprovalMapper.selectOne(query);
        if (record == null) {
            throw new IllegalArgumentException("入库单不存在：" + busNo);
        }
        if (!STATUS_PENDING_APPROVAL.equals(record.getStatusEnum())) {
            throw new IllegalArgumentException(
                    "入库单不在待审批状态，当前状态：" + record.getStatusEnum());
        }
        return record;
    }

    /**
     * lvv：使用“原状态=待审批”作为更新条件，批量更新单据全部明细。
     */
    private int updatePendingOrder(InboundApprovalDTO dto,
                                   String targetStatus,
                                   UserDTO operator,
                                   boolean approvalOperation) {
        LocalDateTime now = LocalDateTime.now();
        String operatorId = operator == null ? null : operator.getId();
        String operatorName = operator == null ? null : operator.getUsername();

        LambdaUpdateWrapper<WkfSupplyRequest> update = new LambdaUpdateWrapper<>();
        update.eq(WkfSupplyRequest::getBusNo, dto.getBusNo())
                .eq(WkfSupplyRequest::getTypeEnum, TYPE_CABINET_INBOUND_RECEIPT)
                .eq(WkfSupplyRequest::getStatusEnum, STATUS_PENDING_APPROVAL)
                .set(WkfSupplyRequest::getStatusEnum, targetStatus)
                .set(WkfSupplyRequest::getRemake, dto.getRemark())
                .set(WkfSupplyRequest::getUpdatedBy, operatorName)
                .set(WkfSupplyRequest::getUpdatedAt, now)
                .setSql("version = version + 1");

        if (approvalOperation) {
            update.set(WkfSupplyRequest::getApproverId, operatorId)
                    .set(WkfSupplyRequest::getApprovalTime, now);
        } else {
            update.set(WkfSupplyRequest::getApproverId, null)
                    .set(WkfSupplyRequest::getApprovalTime, null);
        }
        return inboundApprovalMapper.update(null, update);
    }

    private void requireRemark(InboundApprovalDTO dto, String message) {
        if (!StringUtils.hasText(dto.getRemark())) {
            throw new IllegalArgumentException(message);
        }
    }

    /**
     * lvv：统一清理接口参数，避免空格单号查不到数据或备注保存无效空白。
     */
    private void normalize(InboundApprovalDTO dto) {
        if (dto == null || !StringUtils.hasText(dto.getBusNo())) {
            throw new IllegalArgumentException("入库单号不能为空");
        }
        dto.setBusNo(dto.getBusNo().trim());
        if (dto.getRemark() != null) {
            dto.setRemark(dto.getRemark().trim());
        }
    }

    private void ensureUpdated(int rows, String busNo) {
        if (rows <= 0) {
            throw new IllegalArgumentException("审批流转失败，单据可能已被其他操作处理：" + busNo);
        }
    }

    /**
     * lvv：兼容本地直连调试；经网关调用时会记录真实操作人。
     */
    private UserDTO currentUser() {
        try {
            return userHolder.getCurrentUser();
        } catch (Exception e) {
            log.warn("lvv：未获取到当前用户，按匿名调试请求处理：{}", e.getMessage());
            return null;
        }
    }
}
