package com.zeroone.star.dispense.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundInitVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;

/**
 * <p>
 * 住院退药服务
 * </p>
 *
 * @author jacobzp
 * @since 2026-07-26
 */
public interface IInpatientRefundMedicineService {

    /**
     * 加载页面初始化选项（病区、退药状态）
     */
    InpatientRefundInitVO loadInit();

    /**
     * 分页查询退药患者队列
     */
    PageDTO<InpatientRefundPatientVO> queryPatientQueue(InpatientRefundPatientQuery query);
}
