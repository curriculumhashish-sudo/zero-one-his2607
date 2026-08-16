package com.zeroone.star.dispense.service;

import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseRejectDTO;
import com.zeroone.star.project.query.j7.dispense.DispenseRejectQuery;
import com.zeroone.star.project.query.j7.dispense.DispensingSummaryQuery;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationVO;

import java.util.List;

/**
 * <p>
 * 描述：住院发药管理 服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zg
 * @version 1.0.0
 */
public interface IDispensingService {

    /**
     * 获取汇总单队列（支持条件筛选与分页）
     * @param query 查询条件
     * @return 分页的汇总单列表
     */
    ExtendPageDTO<DispensingSummaryVO> querySummaryList(DispensingSummaryQuery query);

    /**
     * 获取患者药品明细
      * @param summaryNo 汇总单编号
     * @return 患者药品项目列表
     */
    List<PatientMedicationVO> queryPatientMedications(String summaryNo);

    /**
     * 核对发药（确认发药并扣减库存）
     * @param verifyDTO 核对发药请求
     * @return 操作结果
     */
    String verifyDispense(DispenseVerifyDTO verifyDTO);

    // ==================== 住院发药驳回 ====================
    /**
     * 驳回患者发药单据，同步驳回汇总单
     */
    void rejectDispensing(DispenseRejectQuery dto);
}