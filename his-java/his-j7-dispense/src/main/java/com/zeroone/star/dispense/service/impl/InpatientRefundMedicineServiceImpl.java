package com.zeroone.star.dispense.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.mapper.InpatientRefundMedicineMapper;
import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

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
}
