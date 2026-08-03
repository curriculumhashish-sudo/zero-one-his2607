package com.zeroone.star.dispense.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;

import java.util.List;

/**
 * <p>
 * 描述：门诊退药服务接口
 * </p>
 * <p>
 * 功能说明：实现门诊退药业务功能，包括：
 * 1. 分页查询门诊退药患者队列（待退药/已退药）
 * 2. 获取患者退药明细信息
 * 3. 确认退药（回补库存）
 * 涉及的表包括：
 * - med_medication_dispense（药品发放记录表）：退药明细
 * - wkf_inventory_item（库存项表）：库存回补
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface IOutpatientRefundService {

    /**
     * 分页查询门诊退药患者队列
     * <p>
     * 查询 med_medication_dispense 表中发放类型为"退药"(REFUND)的记录，
     * 按状态和创建时间条件过滤，分页返回。
     * 状态为"待处理"(PENDING)表示待退药，状态为"已完成"(REFUNDED)表示已退药。
     * </p>
     *
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    IPage<PatientQueueVO> queryRefundPatientQueue(PatientQueueQuery query);

    /**
     * 获取患者退药明细信息
     * <p>
     * 查询 med_medication_dispense 表中指定患者的所有退药明细记录。
     * </p>
     *
     * @param query 查询参数（包含患者ID、就诊ID等）
     * @return 药品明细列表
     */
    List<PatientDrugItemVO> queryRefundDrugItems(PatientDrugItemQuery query);

    /**
     * 执行确认退药（回补库存）
     * <p>
     * 根据勾选的药品明细ID列表，回补 wkf_inventory_item 表库存数量，
     * 并更新 med_medication_dispense 表状态为"已退药"(REFUNDED)。
     * 业务流程：
     * 1. 校验退药明细存在且状态为"待处理"(PENDING)
     * 2. 根据明细ID查询 med_medication_dispense 获取药品信息
     * 3. 根据药品ID、批号查询 wkf_inventory_item 库存项
     * 4. 回补库存数量（增加 quantity）
     * 5. 更新 med_medication_dispense 状态为"已退药"(REFUNDED)
     * </p>
     *
     * @param dto 退药确认参数
     * @return 操作结果
     */
    String confirmRefund(DispenseVerifyDTO dto);

    /**
     * 执行拒绝退药
     */
    void rejectReturn(ReturnRejectDTO dto);
}
