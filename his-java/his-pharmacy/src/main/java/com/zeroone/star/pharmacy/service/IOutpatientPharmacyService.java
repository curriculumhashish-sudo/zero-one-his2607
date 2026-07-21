package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.pharmacy.PatientQueueVO;

import java.util.List;

/**
 * <p>
 * 描述：门诊药房服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface IOutpatientPharmacyService {

    /**
     * 分页查询门诊退药患者队列
     */
    PageDTO<PatientQueueVO> queryOutpatientReturnQueue(PatientQueueQuery query);

    /**
     * 获取患者退药明细信息
     */
    List<DrugItemVO> getReturnDrugItems(String returnId);

    /**
     * 执行拒绝退药
     */
    void rejectReturn(ReturnRejectDTO dto);
}
