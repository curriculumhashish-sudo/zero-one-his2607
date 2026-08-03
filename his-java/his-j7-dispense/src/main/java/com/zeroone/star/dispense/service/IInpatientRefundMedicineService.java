package com.zeroone.star.dispense.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;

import java.util.List;

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
     * 分页查询退药患者队列
     */
    PageDTO<InpatientRefundPatientVO> queryPatientQueue(InpatientRefundPatientQuery query);

    /**
     * 查询患者退药明细
     */
    List<InpatientRefundDetailVO> queryRefundDetail(InpatientRefundDetailQuery query);

    // ==================== 住院退药确认 ====================
    /**
     * 确认退药（对勾选药品执行退药，同步回补药品库存）
     */
    void confirmReturn(ReturnConfirmDTO dto);
}
