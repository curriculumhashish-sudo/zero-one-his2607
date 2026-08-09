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
    private static final String STATUS_WITHDRAWN = "WITHDRAWN";
    private static final String STATUS_APPROVED = "APPROVED";
    private static final String STATUS_REJECTED = "REJECTED";

    @Resource
    private InboundApprovalMapper inboundApprovalMapper;

    @Resource
    private UserHolder userHolder;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalVO approveApproval(InboundApprovalDTO dto) {
        normalize(dto);
        requirePendingInboundOrder(dto.getBusNo());
        UserDTO operator = requireCurrentUser();
        LocalDateTime now = dto.getApprovalTime() != null ? dto.getApprovalTime() : LocalDateTime.now();
        String approverId = StringUtils.hasText(dto.getApproverId()) ? dto.getApproverId() : operator.getId();
        int rows = updatePendingOrder(dto, STATUS_APPROVED, operator, true, approverId, now);
        ensureUpdated(rows, dto.getBusNo());
        log.info("lvv：入库单 {} 审批通过，更新明细 {} 行", dto.getBusNo(), rows);
        InboundApprovalVO vo = new InboundApprovalVO(dto.getBusNo(), STATUS_APPROVED, "审批通过");
        vo.setApproverName(operator.getUsername());
        vo.setApproveTime(now);
        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalVO rejectApproval(InboundApprovalDTO dto) {
        normalize(dto);
        requireRemark(dto, "驳回原因不能为空");
        requirePendingInboundOrder(dto.getBusNo());
        UserDTO operator = requireCurrentUser();
        LocalDateTime now = dto.getApprovalTime() != null ? dto.getApprovalTime() : LocalDateTime.now();
        String approverId = StringUtils.hasText(dto.getApproverId()) ? dto.getApproverId() : operator.getId();
        int rows = updatePendingOrder(dto, STATUS_REJECTED, operator, true, approverId, now);
        ensureUpdated(rows, dto.getBusNo());
        log.info("lvv：入库单 {} 已驳回，更新明细 {} 行", dto.getBusNo(), rows);
        InboundApprovalVO vo = new InboundApprovalVO(dto.getBusNo(), STATUS_REJECTED, "驳回审批成功");
        vo.setApproverName(operator.getUsername());
        vo.setApproveTime(now);
        return vo;
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
                                   boolean approvalOperation,
                                   String approverId,
                                   LocalDateTime approvalTime) {
        LocalDateTime now = LocalDateTime.now();
        String operatorName = operator.getUsername();

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
            update.set(WkfSupplyRequest::getApproverId, approverId)
                    .set(WkfSupplyRequest::getApprovalTime, approvalTime);
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
     * lvv：审批流转必须记录真实操作人，禁止匿名修改单据状态。
     */
    private UserDTO requireCurrentUser() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser == null || !StringUtils.hasText(currentUser.getId())) {
                throw new IllegalArgumentException("未获取到当前登录用户，无法执行审批流转");
            }
            return currentUser;
        } catch (IllegalArgumentException e) {
            throw e;
        } catch (Exception e) {
            log.warn("lvv：获取当前登录用户失败：{}", e.getMessage());
            throw new IllegalArgumentException("未获取到当前登录用户，无法执行审批流转");
        }
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalVO withdrawApproval(InboundApprovalDTO dto) {
        normalize(dto);
        requireRemark(dto, "撤回原因不能为空");
        WkfSupplyRequest record = requirePendingInboundOrder(dto.getBusNo());
        UserDTO operator = requireCurrentUser();

        // lvv：撤回必须由原申请人本人操作。
        if (StringUtils.hasText(record.getApplicantId())
                && !operator.getId().equals(record.getApplicantId())) {
            throw new IllegalArgumentException("只能由申请人撤回该入库单：" + dto.getBusNo());
        }

        int rows = updatePendingOrder(dto, STATUS_WITHDRAWN, operator, false, null, null);
        ensureUpdated(rows, dto.getBusNo());
        log.info("lvv：入库单 {} 已撤回，更新明细 {} 行", dto.getBusNo(), rows);
        return new InboundApprovalVO(dto.getBusNo(), STATUS_WITHDRAWN, "撤回审批成功");
    }
}
