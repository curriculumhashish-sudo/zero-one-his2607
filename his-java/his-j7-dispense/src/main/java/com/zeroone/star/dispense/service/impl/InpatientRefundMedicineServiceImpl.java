package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.dispense.entity.AdmLocation;
import com.zeroone.star.dispense.mapper.AdmLocationMapper;
import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.project.vo.OptionItemVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundInitVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.Arrays;
import java.util.List;
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

    private static final List<OptionItemVO> REFUND_STATUS_OPTIONS = Arrays.asList(
            new OptionItemVO("PENDING_REFUND", "待退药"),
            new OptionItemVO("REFUNDED", "已退药")
    );

    @Resource
    private AdmLocationMapper admLocationMapper;

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
}
