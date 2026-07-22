package com.zeroone.star.project.j7.refundmedicine;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j7.refundmedicine.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.refundmedicine.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.refundmedicine.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.refundmedicine.InpatientRefundPatientVO;

import java.util.List;

/**
 * 住院退药查询接口
 */
public interface InpatientRefundMedicineApis {

    /** 获取待退药患者队列 */
    JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(InpatientRefundPatientQuery query);

    /** 获取患者退药明细 */
    JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(InpatientRefundDetailQuery query);
}