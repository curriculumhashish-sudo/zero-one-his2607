package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IInboundReceiptService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptDeleteDTO;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptQuery;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailLineVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptListVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.util.List;

/**
 * 入库单业务服务实现（列表 / 详情 / 删除）
 *
 * @author BMXB
 * @since 2026-07-28
 */
@Service
public class InboundReceiptServiceImpl implements IInboundReceiptService {

    /** 入库单类型编码，与审批服务常量一致 */
    private static final String TYPE_INBOUND_RECEIPT = "CABINET_INBOUND_RECEIPT";

    @Resource
    private WkfSupplyRequestMapper supplyRequestMapper;

    @Resource
    private UserHolder userHolder;

    // ==================== 1. 条件分页查询列表 ====================

    @Override
    public List<InboundReceiptListVO> queryAllInboundReceipt(InboundReceiptQuery query) {
        Page<InboundReceiptListVO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        IPage<InboundReceiptListVO> result = supplyRequestMapper.selectInboundReceiptListPage(page, query);
        return result.getRecords();
    }

    // ==================== 2. 查询单据详情 ====================

    @Override
    public InboundReceiptDetailVO getInboundReceiptDetail(InboundReceiptDetailQuery query) {
        // 获取所有明细行（含关联的物料名称/规格/厂商）
        List<InboundReceiptDetailLineVO> lines =
                supplyRequestMapper.selectInboundReceiptDetailLines(query.getBusNo());
        if (lines.isEmpty()) {
            return new InboundReceiptDetailVO();
        }

        // 获取实体行，用于组装头信息
        List<WkfSupplyRequest> rows =
                supplyRequestMapper.selectInboundRowsForApproval(query.getBusNo(), TYPE_INBOUND_RECEIPT);
        if (rows.isEmpty()) {
            return new InboundReceiptDetailVO();
        }

        WkfSupplyRequest header = rows.get(0);

        InboundReceiptDetailVO vo = new InboundReceiptDetailVO();
        vo.setBusNo(header.getBusNo());
        vo.setOriginalBusNo(header.getOriginalBusNo());
        vo.setTypeEnum(header.getTypeEnum());
        vo.setStatusEnum(header.getStatusEnum());
        vo.setSupplierId(header.getSupplierId());
        vo.setSourceLocationId(header.getSourceLocationId());
        vo.setPurposeLocationId(header.getPurposeLocationId());
        vo.setSourceTypeEnum(header.getSourceTypeEnum());
        vo.setPurposeTypeEnum(header.getPurposeTypeEnum());
        vo.setCategoryEnum(header.getCategoryEnum());
        vo.setRowVersion(header.getVersion());
        vo.setApplyTime(header.getApplyTime());
        vo.setApplicantId(header.getApplicantId());
        vo.setApprovalTime(header.getApprovalTime());
        vo.setReason(header.getReason());
        vo.setLines(lines);

        return vo;
    }

    // ==================== 3. 删除单据 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String removeInboundReceipt(InboundReceiptDeleteDTO dto) {
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();

        int affected = supplyRequestMapper.deleteInboundByBusNo(dto.getBusNo(), now, userId);
        if (affected > 0) {
            return "删除成功";
        } else {
            throw new IllegalStateException("删除失败，单据不存在或已被删除");
        }
    }

    // ==================== 辅助 ====================

    private String currentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            if (user != null && StringUtils.hasText(user.getId())) {
                return user.getId();
            }
        } catch (Exception ignored) {
            // 无网关 header 时容错
        }
        return "system";
    }
}
