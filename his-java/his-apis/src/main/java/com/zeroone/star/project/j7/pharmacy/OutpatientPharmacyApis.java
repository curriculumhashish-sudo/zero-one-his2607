package com.zeroone.star.project.j7.pharmacy;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;

import java.util.List;

/**
 * <p>
 * 描述：门诊药房API接口定义
 * </p>
 * <p>
 * 功能说明：定义门诊药房对外提供的 RESTful API 接口规范，
 * 包括门诊拒绝退药业务模块：
 * 1. 分页查询门诊退药患者队列
 * 2. 查看患者退药明细
 * 3. 执行拒绝退药
 * 该接口由 his-pharmacy 模块的 OutpatientController 实现。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface OutpatientPharmacyApis {

    // ==================== 门诊拒绝退药 ====================

    /**
     * 分页查询门诊退药患者队列
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    JsonVO<PageDTO<PatientQueueVO>> queryOutpatientReturnQueue(PatientQueueQuery query);

    /**
     * 获取患者退药明细信息
     * @param returnId 退药申请单ID
     * @return 药品明细列表
     */
    JsonVO<List<DrugItemVO>> getReturnDrugItems(String returnId);

    /**
     * 执行拒绝退药
     * @param dto 拒绝退药参数
     * @return 操作结果
     */
    JsonVO<String> rejectReturn(ReturnRejectDTO dto);
}
