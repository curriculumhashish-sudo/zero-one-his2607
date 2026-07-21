package com.zeroone.star.pharmacy.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.pharmacy.entity.*;
import com.zeroone.star.pharmacy.mapper.*;
import com.zeroone.star.pharmacy.service.IInpatientPharmacyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.pharmacy.DispensingRejectDTO;
import com.zeroone.star.project.dto.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.query.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.pharmacy.PatientQueueVO;
import com.zeroone.star.project.vo.pharmacy.SummaryVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：住院药房服务实现类
 * </p>
 * <p>
 * 功能说明：实现住院发药驳回和住院退药确认两大业务功能。
 * 涉及的表包括：
 * - dispensing_header（发药单据主表）：发药状态管理
 * - dispensing_item（发药明细表）：药品项目查询
 * - return_header（退药申请主表）：退药状态管理
 * - return_item（退药申请明细表）：退药明细查询
 * - drug_stock（药品库存表）：退药时库存回补
 * - summary_order（汇总单表）：汇总单状态同步
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Service
public class InpatientPharmacyServiceImpl extends ServiceImpl<DispensingHeaderMapper, DispensingHeader> implements IInpatientPharmacyService {

    @Resource
    private DispensingHeaderMapper dispensingHeaderMapper;

    @Resource
    private DispensingItemMapper dispensingItemMapper;

    @Resource
    private ReturnHeaderMapper returnHeaderMapper;

    @Resource
    private ReturnItemMapper returnItemMapper;

    @Resource
    private DrugStockMapper drugStockMapper;

    @Resource
    private SummaryOrderMapper summaryOrderMapper;

    // ==================== 住院发药驳回 ====================

    /**
     * 获取汇总单队列
     * <p>
     * 查询 summary_order 表中状态为"待发药"（status=0）的汇总单列表，
     * 按创建时间倒序排列，返回给前端展示。
     * </p>
     *
     * @return 汇总单视图对象列表
     */
    @Override
    public List<SummaryVO> getSummaryList() {
        // 查询 summary_order 表：状态=待发药，按创建时间倒序
        List<SummaryOrder> list = summaryOrderMapper.selectList(
                new LambdaQueryWrapper<SummaryOrder>()
                        .eq(SummaryOrder::getStatus, 0) // 仅查询待发药的汇总单
                        .orderByDesc(SummaryOrder::getCreateTime)
        );
        return list.stream().map(item -> {
            SummaryVO vo = new SummaryVO();
            BeanUtil.copyProperties(item, vo);
            vo.setSummaryId(item.getId());
            return vo;
        }).collect(Collectors.toList());
    }

    /**
     * 根据汇总单ID获取患者药品项目
     * <p>
     * 先查询 dispensing_header 表获取该汇总单下的所有发药单据，
     * 再根据发药单据ID列表查询 dispensing_item 表获取药品明细。
     * </p>
     *
     * @param summaryId 汇总单ID
     * @return 药品项目视图对象列表
     */
    @Override
    public List<DrugItemVO> getDrugItemsBySummary(String summaryId) {
        // 第一步：根据汇总单ID查询 dispensing_header 表获取发药单据
        List<DispensingHeader> headers = dispensingHeaderMapper.selectList(
                new LambdaQueryWrapper<DispensingHeader>()
                        .eq(DispensingHeader::getSummaryId, summaryId)
        );
        if (headers.isEmpty()) {
            return Collections.emptyList();
        }
        // 第二步：收集所有发药单据ID
        List<String> dispensingIds = headers.stream()
                .map(DispensingHeader::getId)
                .collect(Collectors.toList());
        // 第三步：根据发药单据ID列表查询 dispensing_item 表获取药品明细
        List<DispensingItem> items = dispensingItemMapper.selectList(
                new LambdaQueryWrapper<DispensingItem>()
                        .in(DispensingItem::getDispensingId, dispensingIds)
        );
        return items.stream().map(item -> {
            DrugItemVO vo = new DrugItemVO();
            BeanUtil.copyProperties(item, vo);
            vo.setItemId(item.getId());
            return vo;
        }).collect(Collectors.toList());
    }

    /**
     * 执行发药核对（发药后截单）
     * <p>
     * 更新 dispensing_header 表中指定发药单据的状态为"已发药"（status=1）。
     * 此操作在事务中执行，确保数据一致性。
     * </p>
     *
     * @param dispensingId 发药单据ID
     */
    @Override
    @Transactional
    public void confirmDispensing(String dispensingId) {
        // 更新 dispensing_header 表：设置状态=已发药
        dispensingHeaderMapper.update(null,
                new LambdaUpdateWrapper<DispensingHeader>()
                        .eq(DispensingHeader::getId, dispensingId)
                        .set(DispensingHeader::getStatus, 1)
        );
    }

    /**
     * 驳回患者发药单据，同步驳回汇总单
     * <p>
     * 两步操作在同一个事务中执行：
     * 1. 更新 dispensing_header 表：设置状态="已驳回"（status=2），记录驳回原因
     * 2. 更新 summary_order 表：设置状态="已驳回"（status=2），同步驳回汇总单
     * </p>
     *
     * @param dto 发药驳回参数（包含发药单据ID、汇总单ID、驳回原因等）
     */
    @Override
    @Transactional
    public void rejectDispensing(DispensingRejectDTO dto) {
        // 1. 更新 dispensing_header 表：状态=已驳回，记录驳回原因
        dispensingHeaderMapper.update(null,
                new LambdaUpdateWrapper<DispensingHeader>()
                        .eq(DispensingHeader::getId, dto.getDispensingId())
                        .set(DispensingHeader::getStatus, 2)
                        .set(DispensingHeader::getRejectReason, dto.getRejectReason())
        );
        // 2. 同步驳回汇总单 - 更新 summary_order 表：状态=已驳回
        if (dto.getSummaryId() != null) {
            summaryOrderMapper.update(null,
                    new LambdaUpdateWrapper<SummaryOrder>()
                            .eq(SummaryOrder::getId, dto.getSummaryId())
                            .set(SummaryOrder::getStatus, 2)
            );
        }
    }

    // ==================== 住院退药确认 ====================

    /**
     * 分页查询退药患者队列
     * <p>
     * 查询 return_header 表中退药类型为"住院退药"（returnType=0）且
     * 状态为"待确认"（status=0）的记录，支持按患者姓名模糊查询，
     * 按创建时间倒序排列，分页返回。
     * </p>
     *
     * @param query 分页查询参数（支持按患者姓名模糊查询）
     * @return 患者队列分页数据
     */
    @Override
    public PageDTO<PatientQueueVO> queryReturnPatientQueue(PatientQueueQuery query) {
        Page<ReturnHeader> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<ReturnHeader> wrapper = new LambdaQueryWrapper<ReturnHeader>()
                .eq(ReturnHeader::getReturnType, 0) // 住院退药
                .eq(ReturnHeader::getStatus, 0)     // 待确认
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
        return items.stream().map(item -> {
            DrugItemVO vo = new DrugItemVO();
            BeanUtil.copyProperties(item, vo);
            vo.setItemId(item.getId());
            return vo;
        }).collect(Collectors.toList());
    }

    /**
     * 确认退药（对勾选药品执行退药，同步回补药品库存）
     * <p>
     * 两步操作在同一个事务中执行：
     * 1. 更新 return_header 表：设置状态="已确认"（status=1），记录确认人
     * 2. 对勾选的药品回补库存：查询 drug_stock 表找到对应药品，
     *    将库存数量增加退药数量（stock_quantity + quantity），实现库存回补
     * </p>
     *
     * @param dto 退药确认参数（包含退药申请单ID、勾选的药品明细ID列表、操作人）
     */
    @Override
    @Transactional
    public void confirmReturn(ReturnConfirmDTO dto) {
        // 1. 更新 return_header 表：状态=已确认，记录确认人
        returnHeaderMapper.update(null,
                new LambdaUpdateWrapper<ReturnHeader>()
                        .eq(ReturnHeader::getId, dto.getReturnId())
                        .set(ReturnHeader::getStatus, 1)
                        .set(ReturnHeader::getConfirmBy, dto.getOperator())
        );

        // 2. 对勾选的药品回补库存
        if (dto.getItemIds() != null && !dto.getItemIds().isEmpty()) {
            // 查询 return_item 表获取勾选药品的详细信息
            List<ReturnItem> returnItems = returnItemMapper.selectList(
                    new LambdaQueryWrapper<ReturnItem>()
                            .in(ReturnItem::getId, dto.getItemIds())
            );
            // 遍历每个药品，回补库存
            for (ReturnItem item : returnItems) {
                // 查询 drug_stock 表：根据药品编码查找库存记录
                DrugStock stock = drugStockMapper.selectOne(
                        new LambdaQueryWrapper<DrugStock>()
                                .eq(DrugStock::getDrugCode, item.getDrugCode())
                                .last("LIMIT 1")
                );
                if (stock != null) {
                    // 更新 drug_stock 表：库存数量 = 原库存 + 退药数量
                    drugStockMapper.update(null,
                            new LambdaUpdateWrapper<DrugStock>()
                                    .eq(DrugStock::getId, stock.getId())
                                    .set(DrugStock::getStockQuantity,
                                            stock.getStockQuantity() + item.getQuantity())
                    );
                }
            }
        }
    }
}
