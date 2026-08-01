package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WkfSupplyDelivery;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfSupplyDeliveryMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IOutpatientPharmacyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：门诊药房服务实现类
 * </p>
 * <p>
 * 功能说明：实现门诊拒绝退药业务功能。
 * 涉及的表包括：
 * - wkf_supply_request（供应请求表）：退药主单据，退药类型为"门诊退药"
 * - wkf_supply_delivery（供应交付表）：退药明细
 * </p>
 * <p>
 * 版权：&copy;01星球
 * </p>
 * <p>
 * 地址：01星球总部
 * </p>
 * 
 * @author 01star
 * @version 1.0.0
 */
@Service
public class OutpatientPharmacyServiceImpl implements IOutpatientPharmacyService {

    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Resource
    private WkfSupplyDeliveryMapper wkfSupplyDeliveryMapper;

    /**
     * 分页查询门诊退药患者队列
     * <p>
     * 查询 wkf_supply_request 表中退药类型为"门诊退药"、状态为"待处理"的记录，
     * 按创建时间倒序排列，分页返回。
     * </p>
     *
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    @Override
    public PageDTO<PatientQueueVO> queryOutpatientReturnQueue(PatientQueueQuery query) {
        Page<WkfSupplyRequest> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<WkfSupplyRequest> wrapper = new LambdaQueryWrapper<WkfSupplyRequest>()
                .eq(WkfSupplyRequest::getTypeEnum, "RETURN_OUTPATIENT")
                .eq(WkfSupplyRequest::getStatusEnum, "PENDING")
                .orderByDesc(WkfSupplyRequest::getCreatedAt);
        Page<WkfSupplyRequest> result = wkfSupplyRequestMapper.selectPage(page, wrapper);
        // 手动映射到 PatientQueueVO
        Page<PatientQueueVO> voPage = new Page<>(result.getCurrent(), result.getSize(), result.getTotal());
        voPage.setRecords(result.getRecords().stream().map(item -> {
            PatientQueueVO vo = new PatientQueueVO();
            vo.setPatientId(item.getPatientId());
            vo.setStatus(0);
            return vo;
        }).collect(Collectors.toList()));
        return PageDTO.create(voPage, vo -> vo);
    }

    /**
     * 获取患者退药明细信息
     * <p>
     * 查询 wkf_supply_delivery 表中指定供应请求ID的所有交付明细。
     * </p>
     *
     * @param returnId 退药请求ID（对应 wkf_supply_delivery.request_id）
     * @return 药品明细列表
     */
    @Override
    public List<DrugItemVO> getReturnDrugItems(String returnId) {
        List<WkfSupplyDelivery> deliveries = wkfSupplyDeliveryMapper.selectList(
                new LambdaQueryWrapper<WkfSupplyDelivery>()
                        .eq(WkfSupplyDelivery::getRequestId, returnId));
        if (deliveries.isEmpty()) {
            return Collections.emptyList();
        }
        return deliveries.stream().map(item -> {
            DrugItemVO vo = new DrugItemVO();
            vo.setItemId(item.getId());
            vo.setDrugCode(item.getItemId());
            vo.setQuantity(item.getQuantity() != null ? item.getQuantity().intValue() : null);
            vo.setUnit(item.getUnitCode());
            vo.setBatchNo(item.getLotNumber());
            return vo;
        }).collect(Collectors.toList());
    }

    /**
     * 执行拒绝退药
     * <p>
     * 更新 wkf_supply_request 表：设置状态为"已拒绝"，记录拒绝原因。
     * 同时校验退药类型为"门诊退药"，确保不会误操作住院退药。
     * </p>
     *
     * @param dto 拒绝退药参数
     */
    @Override
    @Transactional
    public void rejectReturn(ReturnRejectDTO dto) {
        wkfSupplyRequestMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getId, dto.getReturnId())
                        .eq(WkfSupplyRequest::getTypeEnum, "RETURN_OUTPATIENT")
                        .set(WkfSupplyRequest::getStatusEnum, "REJECTED")
                        .set(WkfSupplyRequest::getReason, dto.getRejectReason()));
    }
}
