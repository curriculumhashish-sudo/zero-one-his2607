package com.zeroone.star.project.j7.dispense;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.InpatientRefundConfirmDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundConfirmVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundInitVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;

import java.util.List;

/**
 * <p>
 * 描述：住院退药接口（对齐原型：初始化、患者队列、退药明细、确认退药）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author jacobzp
 * @version 1.0.0
 */
public interface InpatientRefundMedicineApis {

    /** 加载页面初始化选项 */
    JsonVO<InpatientRefundInitVO> loadInit();

    /** 分页查询退药患者队列 */
    JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(InpatientRefundPatientQuery query);

    /** 获取患者退药明细 */
    JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(InpatientRefundDetailQuery query);

    /** 确认退药（当前阶段假数据） */
    JsonVO<InpatientRefundConfirmVO> confirmReturn(InpatientRefundConfirmDTO dto);
}
