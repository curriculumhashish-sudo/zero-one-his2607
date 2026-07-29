package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.DispensingRejectDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
import com.zeroone.star.project.vo.j7.pharmacy.SummaryVO;

import java.util.List;

/**
 * <p>
 * 描述：住院药房服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface IInpatientPharmacyService {

    // ==================== 住院发药驳回 ====================

    /**
     * 获取汇总单队列
     */
    List<SummaryVO> getSummaryList();

    /**
     * 根据汇总单ID获取患者药品项目
     */
    List<DrugItemVO> getDrugItemsBySummary(String summaryId);

    /**
     * 执行发药核对（发药后截单）
     */
    void confirmDispensing(String dispensingId);

    /**
     * 驳回患者发药单据，同步驳回汇总单
     */
    void rejectDispensing(DispensingRejectDTO dto);

    // ==================== 住院退药确认 ====================

    /**
     * 分页查询退药患者队列
     */
    PageDTO<PatientQueueVO> queryReturnPatientQueue(PatientQueueQuery query);

    /**
     * 获取患者退药明细信息
     */
    List<DrugItemVO> getReturnDrugItems(String returnId);

    /**
     * 确认退药（对勾选药品执行退药，同步回补药品库存）
     */
    void confirmReturn(ReturnConfirmDTO dto);
}
