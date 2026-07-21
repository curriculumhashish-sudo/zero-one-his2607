package com.zeroone.star.pharmacy.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.ReturnHeader;
import com.zeroone.star.pharmacy.entity.ReturnItem;
import com.zeroone.star.pharmacy.mapper.ReturnHeaderMapper;
import com.zeroone.star.pharmacy.mapper.ReturnItemMapper;
import com.zeroone.star.pharmacy.service.IOutpatientPharmacyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.pharmacy.PatientQueueVO;
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
 * - return_header（退药申请主表）：退药单状态管理
 * - return_item（退药申请明细表）：退药明细查询
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Service
public class OutpatientPharmacyServiceImpl implements IOutpatientPharmacyService {

    @Resource
    private ReturnHeaderMapper returnHeaderMapper;

    @Resource
    private ReturnItemMapper returnItemMapper;

    /**
     * 分页查询门诊退药患者队列
     * <p>
     * 查询 return_header 表中退药类型为"门诊退药"（returnType=1）且
     * 状态为"待处理"（status=0）的记录，支持按患者姓名模糊查询，
     * 按创建时间倒序排列，分页返回。
     * </p>
     *
     * @param query 分页查询参数（支持按患者姓名模糊查询）
     * @return 患者队列分页数据
     */
    @Override
    public PageDTO<PatientQueueVO> queryOutpatientReturnQueue(PatientQueueQuery query) {
        Page<ReturnHeader> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<ReturnHeader> wrapper = new LambdaQueryWrapper<ReturnHeader>()
                .eq(ReturnHeader::getReturnType, 1) // 门诊退药
                .eq(ReturnHeader::getStatus, 0)     // 待处理
                .like(query.getPatientName() != null && !query.getPatientName().isEmpty(),
                        ReturnHeader::getPatientName, query.getPatientName())
                .orderByDesc(ReturnHeader::getCreateTime);
        Page<ReturnHeader> result = returnHeaderMapper.selectPage(page, wrapper);
        return PageDTO.create(result, PatientQueueVO::new);
    }

    /**
     * 获取患者退药明细信息
     * <p>
     * 查询 return_item 表中指定退药申请单ID（returnId）的所有药品明细。
     * </p>
     *
     * @param returnId 退药申请单ID
     * @return 药品明细列表
     */
    @Override
    public List<DrugItemVO> getReturnDrugItems(String returnId) {
        // 查询 return_item 表：根据退药申请单ID获取药品明细
        List<ReturnItem> items = returnItemMapper.selectList(
                new LambdaQueryWrapper<ReturnItem>()
                        .eq(ReturnItem::getReturnId, returnId)
        );
        if (items.isEmpty()) {
            return Collections.emptyList();
        }
        return items.stream().map(item -> {
            DrugItemVO vo = new DrugItemVO();
            BeanUtil.copyProperties(item, vo);
            vo.setItemId(item.getId());
            return vo;
        }).collect(Collectors.toList());
    }

    /**
     * 执行拒绝退药
     * <p>
     * 更新 return_header 表：设置状态="已拒绝"（status=3），记录拒绝原因。
     * 同时检查退药类型为"门诊退药"（returnType=1），确保不会误操作住院退药。
     * 此操作在事务中执行，确保数据一致性。
     * </p>
     *
     * @param dto 拒绝退药参数（包含退药申请单ID、拒绝原因等）
     */
    @Override
    @Transactional
    public void rejectReturn(ReturnRejectDTO dto) {
        // 更新 return_header 表：状态=已拒绝，记录拒绝原因
        returnHeaderMapper.update(null,
                new LambdaUpdateWrapper<ReturnHeader>()
                        .eq(ReturnHeader::getId, dto.getReturnId())
                        .eq(ReturnHeader::getReturnType, 1) // 确保是门诊退药
                        .set(ReturnHeader::getStatus, 3)    // 已拒绝
                        .set(ReturnHeader::getRejectReason, dto.getRejectReason())
        );
    }
}
