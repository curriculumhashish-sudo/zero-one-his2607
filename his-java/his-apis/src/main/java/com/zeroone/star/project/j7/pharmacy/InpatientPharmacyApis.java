package com.zeroone.star.project.j7.pharmacy;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseRejectDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
import com.zeroone.star.project.vo.j7.pharmacy.SummaryVO;

import java.util.List;

/**
 * <p>
 * 描述：住院药房API接口定义
 * </p>
 * <p>
 * 功能说明：定义住院药房对外提供的 RESTful API 接口规范，
 * 包括住院发药驳回和住院退药确认两大业务模块：
 * 1. 住院发药驳回：获取汇总单队列 → 查看药品项目 → 发药核对 → 驳回发药
 * 2. 住院退药确认：查询退药患者队列 → 查看退药明细 → 确认退药
 * 该接口由 his-pharmacy 模块的 InpatientController 实现。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface InpatientPharmacyApis {

    // ==================== 住院发药驳回 ====================

    /**
     * 获取汇总单队列
     * @return 汇总单列表
     */
    JsonVO<List<SummaryVO>> getSummaryList();

    /**
     * 根据汇总单ID获取患者药品项目
     * @param summaryId 汇总单ID
     * @return 药品项目列表
     */
    JsonVO<List<DrugItemVO>> getDrugItemsBySummary(String summaryId);

    /**
     * 执行发药核对（发药后截单）
     * @param dispensingId 发药单据ID
     * @return 操作结果
     */
    JsonVO<String> confirmDispensing(String dispensingId);

    /**
     * 驳回患者发药单据
     * @param dto 驳回参数
     * @return 操作结果
     */
    JsonVO<String> rejectDispensing(DispenseRejectDTO dto);

    // ==================== 住院退药确认 ====================

    /**
     * 分页查询退药患者队列
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    JsonVO<PageDTO<PatientQueueVO>> queryReturnPatientQueue(PatientQueueQuery query);

    /**
     * 获取患者退药明细信息
     * @param returnId 退药申请单ID
     * @return 药品明细列表
     */
    JsonVO<List<DrugItemVO>> getReturnDrugItems(String returnId);

    /**
     * 确认退药（对勾选药品执行退药，同步回补药品库存）
     * @param dto 退药确认参数
     * @return 操作结果
     */
    JsonVO<String> confirmReturn(ReturnConfirmDTO dto);
}
