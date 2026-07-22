package com.zeroone.star.project.j7.dispense;

import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.query.j7.dispense.DispensingSummaryQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationVO;

import java.util.List;

/**
 * <p>
 * 描述：住院发药管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zg
 * @version 1.0.0
 */
public interface DispensingApis {

    /**
     * 获取汇总单队列（支持条件筛选与分页）
     * @param query 查询条件
     * @return 分页的汇总单列表
     */
    JsonVO<ExtendPageDTO<DispensingSummaryVO>> querySummaryList(DispensingSummaryQuery query);

    /**
     * 获取患者药品项目（点击汇总单后查看明细）
     * @param summaryId 汇总单ID
     * @return 患者药品项目列表
     */
    JsonVO<List<PatientMedicationVO>> queryPatientMedications(Long summaryId);

    /**
     * 核对发药（确认发药并扣减库存）
     * @param verifyDTO 核对发药请求
     * @return 核对发药结果
     */
    JsonVO<String> verifyDispense(DispenseVerifyDTO verifyDTO);
}
