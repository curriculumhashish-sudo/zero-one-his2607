package com.zeroone.star.project.dispensing;

import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.dispensing.DispenseVerifyDTO;
import com.zeroone.star.project.query.dispensing.DispensingSummaryQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.dispensing.DispensingSummaryVO;
import com.zeroone.star.project.vo.dispensing.PatientMedicationVO;

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

    /**
     * 发药后截单（关闭汇总单，防止后续修改）
     * @param summaryNo 汇总单号
     * @return 操作结果
     */
    JsonVO<String> closeSummary(String summaryNo);
}
