package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.AdmLocation;
import com.zeroone.star.dispense.mapper.AdmLocationMapper;
import com.zeroone.star.dispense.mapper.InpatientRefundMedicineMapper;
import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.OptionItemVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundInitVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import java.util.stream.Collectors;

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

    private static final String FORM_WARD = "WARD";
    private static final String STATUS_ACTIVE = "ACTIVE";
    private static final String QUEUE_TAB_PENDING = "PENDING";
    private static final String QUEUE_TAB_COMPLETED = "COMPLETED";

    private static final List<OptionItemVO> REFUND_STATUS_OPTIONS = Arrays.asList(
            new OptionItemVO("PENDING", "待退药"),
            new OptionItemVO("REFUNDED", "已退药")
    );

    @Resource
    private AdmLocationMapper admLocationMapper;

    @Resource
    private InpatientRefundMedicineMapper inpatientRefundMedicineMapper;

    @Override
    public InpatientRefundInitVO loadInit() {
        /**
         * 加载病区选项 - SQL:
         * SELECT *
         * FROM adm_location
         * WHERE form_enum = 'WARD'
         *   AND status_enum = 'ACTIVE'
         *   AND is_deleted = 0          -- @TableLogic 自动加
         * ORDER BY display_order ASC, name ASC
         */
        List<AdmLocation> wards = admLocationMapper.selectList(
                new LambdaQueryWrapper<AdmLocation>()
                        .eq(AdmLocation::getFormEnum, FORM_WARD)
                        .eq(AdmLocation::getStatusEnum, STATUS_ACTIVE)
                        .orderByAsc(AdmLocation::getDisplayOrder)
                        .orderByAsc(AdmLocation::getName)
        );

        List<OptionItemVO> wardOptions = wards.stream()
                .map(ward -> new OptionItemVO(ward.getId(), ward.getName()))
                .collect(Collectors.toList());

        InpatientRefundInitVO init = new InpatientRefundInitVO();
        init.setWardOptions(wardOptions);
        init.setRefundStatusOptions(REFUND_STATUS_OPTIONS);
        return init;
    }

    @Override
    public PageDTO<InpatientRefundPatientVO> queryPatientQueue(InpatientRefundPatientQuery query) {
        Objects.requireNonNull(query, "查询条件不能为空");
        normalizeQuery(query);

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

    /**
     * 对查询条件进行数据清洗和默认值设置
     */
    private void normalizeQuery(InpatientRefundPatientQuery query) {
        query.setWardId(trimToNull(query.getWardId()));
        query.setKeyword(trimToNull(query.getKeyword()));

        String queueTab = toUpperCase(query.getQueueTab());
        if (queueTab == null || !QUEUE_TAB_COMPLETED.equals(queueTab)) {
            queueTab = QUEUE_TAB_PENDING;
        }
        query.setQueueTab(queueTab);
    }

    /**
     * 将字符串转换为大写，并使用trimToNull进行数据清洗
     */
    private String toUpperCase(String value) {
        // 清洗数据
        String normalized = trimToNull(value);
        // 转换为大写
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
