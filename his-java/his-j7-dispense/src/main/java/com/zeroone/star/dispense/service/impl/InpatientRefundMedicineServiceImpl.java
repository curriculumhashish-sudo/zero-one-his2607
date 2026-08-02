package com.zeroone.star.dispense.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.mapper.InpatientRefundMedicineMapper;
import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyDelivery;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyDeliveryMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.Objects;

/**
 * <p>
 * 住院退药服务实现
 * </p>
 *
 * @author jacobzp
 * @since 2026-07-26
 */
@Service
public class InpatientRefundMedicineServiceImpl implements IInpatientRefundMedicineService {

    private static final String QUEUE_TAB_PENDING = "PENDING";
    private static final String QUEUE_TAB_COMPLETED = "COMPLETED";
    private static final String STATUS_PENDING = "PENDING";
    private static final String STATUS_REFUNDED = "REFUNDED";

    @Resource
    private InpatientRefundMedicineMapper inpatientRefundMedicineMapper;

    @Override
    public PageDTO<InpatientRefundPatientVO> queryPatientQueue(InpatientRefundPatientQuery query) {
        Objects.requireNonNull(query, "查询条件不能为空");
        normalizePatientQuery(query);

        long pageIndex = Math.max(query.getPageIndex(), 1);
        long pageSize = Math.max(query.getPageSize(), 1);
        Page<InpatientRefundPatientVO> page = new Page<>(pageIndex, pageSize);
        inpatientRefundMedicineMapper.selectPatientQueuePage(page, query);

        PageDTO<InpatientRefundPatientVO> result = PageDTO.create(page);
        if (result.getRows() == null) {
            result.setRows(Collections.emptyList());
        }
        return result;
    }

    @Override
    public List<InpatientRefundDetailVO> queryRefundDetail(InpatientRefundDetailQuery query) {
        Objects.requireNonNull(query, "查询条件不能为空");
        if (StrUtil.isBlank(query.getEncounterId())) {
            throw new IllegalArgumentException("就诊ID不能为空");
        }
        normalizeDetailQuery(query);

        List<InpatientRefundDetailVO> rows = inpatientRefundMedicineMapper.selectRefundDetailList(query);
        return rows == null ? Collections.emptyList() : rows;
    }

    private void normalizePatientQuery(InpatientRefundPatientQuery query) {
        query.setWardId(trimToNull(query.getWardId()));
        query.setKeyword(trimToNull(query.getKeyword()));

        String queueTab = toUpperCase(query.getQueueTab());
        if (queueTab == null || !QUEUE_TAB_COMPLETED.equals(queueTab)) {
            queueTab = QUEUE_TAB_PENDING;
        }
        query.setQueueTab(queueTab);
    }

    private void normalizeDetailQuery(InpatientRefundDetailQuery query) {
        query.setEncounterId(trimToNull(query.getEncounterId()));
        String refundStatus = toUpperCase(query.getRefundStatus());
        if (STATUS_REFUNDED.equals(refundStatus)) {
            query.setRefundStatus(STATUS_REFUNDED);
        } else {
            // 空 / PENDING / PENDING_REFUND → Mapper otherwise 分支筛库表 PENDING
            query.setRefundStatus(STATUS_PENDING);
        }
    }

    private String toUpperCase(String value) {
        String normalized = trimToNull(value);
        return normalized == null ? null : normalized.toUpperCase(Locale.ROOT);
    }

    private String trimToNull(String value) {
        if (value == null) {
            return null;
        }
        String trimmed = value.strip();
        return trimmed.isEmpty() ? null : trimmed;
    }

    /**
     * 确认退药（对勾选药品执行退药，同步回补药品库存）
     * <p>
     * 两步操作在同一事务中执行：
     * 1. 更新供应请求状态为"已确认"，记录审批人和审批时间
     * 2. 对勾选的药品回补库存：查询 wkf_inventory_item 找到对应库存记录，
     *    将库存数量增加退药数量
     * </p>
     *
     * @param dto 退药确认参数
     */
    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;
    @Resource
    private WkfSupplyDeliveryMapper wkfSupplyDeliveryMapper;
    @Resource
    private WkfInventoryItemMapper wkfInventoryItemMapper;
    @Override
    @Transactional
    public void confirmReturn(ReturnConfirmDTO dto) {
        // 1. 更新供应请求：状态=已确认，记录审批信息
        wkfSupplyRequestMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getId, dto.getReturnId())
                        .set(WkfSupplyRequest::getStatusEnum, "CONFIRMED")
                        .set(WkfSupplyRequest::getApproverId, dto.getOperator())
                        .set(WkfSupplyRequest::getApprovalTime, LocalDateTime.now())
        );

        // 2. 对勾选的药品回补库存
        if (dto.getItemIds() != null && !dto.getItemIds().isEmpty()) {
            List<WkfSupplyDelivery> returnItems = wkfSupplyDeliveryMapper.selectList(
                    new LambdaQueryWrapper<WkfSupplyDelivery>()
                            .in(WkfSupplyDelivery::getId, dto.getItemIds())
            );
            for (WkfSupplyDelivery item : returnItems) {
                // 查询库存：根据物品表名和物品标识查找库存记录
                WkfInventoryItem stock = wkfInventoryItemMapper.selectOne(
                        new LambdaQueryWrapper<WkfInventoryItem>()
                                .eq(WkfInventoryItem::getItemTable, item.getItemTable())
                                .eq(WkfInventoryItem::getItemId, item.getItemId())
                );
                if (stock != null) {
                    BigDecimal newQty = (stock.getQuantity() != null ? stock.getQuantity() : BigDecimal.ZERO)
                            .add(item.getQuantity() != null ? item.getQuantity() : BigDecimal.ZERO);
                    wkfInventoryItemMapper.update(null,
                            new LambdaUpdateWrapper<WkfInventoryItem>()
                                    .eq(WkfInventoryItem::getId, stock.getId())
                                    .set(WkfInventoryItem::getQuantity, newQty)
                    );
                }
            }
        }
    }
}
