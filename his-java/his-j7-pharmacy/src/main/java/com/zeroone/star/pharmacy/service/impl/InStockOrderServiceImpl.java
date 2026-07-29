package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.InStockOrder;
import com.zeroone.star.pharmacy.mapper.InStockOrderMapper;
import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.util.List;
import java.util.stream.Collectors;

@Service
@RequiredArgsConstructor
public class InStockOrderServiceImpl implements InStockOrderService {

    private final InStockOrderMapper inStockOrderMapper;
    // TODO: 如果有库房字典服务或用户服务，请在此注入
    // private final LocationService locationService; 
    // private final UserService userService; 

    @Override
    public IPage<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query) {
        // 1. 构建查询条件
        LambdaQueryWrapper<InStockOrder> wrapper = new LambdaQueryWrapper<>();
        wrapper.like(StringUtils.hasText(query.getBusNo()), InStockOrder::getBusNo, query.getBusNo())
                .eq(StringUtils.hasText(query.getSourceLocationId()), InStockOrder::getSourceLocationId, query.getSourceLocationId())
                .eq(StringUtils.hasText(query.getPurposeLocationId()), InStockOrder::getPurposeLocationId, query.getPurposeLocationId())
                .eq(StringUtils.hasText(query.getStatusEnum()), InStockOrder::getStatusEnum, query.getStatusEnum())
                .ge(query.getStartTime() != null, InStockOrder::getApplyTime, query.getStartTime())
                .le(query.getEndTime() != null, InStockOrder::getApplyTime, query.getEndTime())
                .orderByDesc(InStockOrder::getApplyTime);

        // 2. 执行分页查询
        // 创建MyBatis-Plus分页对象，使用查询参数中的页码和页面大小来初始化分页信息
        // query.getPageIndex(): 获取当前查询的页码索引（从1开始），用于确定要查询第几页的数据
        // query.getPageSize(): 获取每页显示的数据条数，用于限制每次查询返回的结果数量
        Page<InStockOrder> page = new Page<>(query.getPageIndex(), query.getPageSize());
        // 执行分页查询操作，将构建好的查询条件wrapper应用到数据库查询中，返回分页结果
        IPage<InStockOrder> orderPage = inStockOrderMapper.selectPage(page, wrapper);

        // 3. Entity 转 VO (包含流转路径拼接逻辑)
        return orderPage.convert(this::convertToListVO);
    }

    @Override
    public PurchaseInboundDetailVO getInboundDetail(String id) {
        // 1. 查询单据头
        InStockOrder order = inStockOrderMapper.selectById(id);
        if (order == null) {
            throw new RuntimeException("入库单不存在");
        }

        // 2. 组装详情 VO
        PurchaseInboundDetailVO detailVO = new PurchaseInboundDetailVO();
        detailVO.setId(order.getId());
        detailVO.setBusNo(order.getBusNo());
        detailVO.setSupplierId(order.getSupplierId());
        detailVO.setPurposeLocationId(order.getPurposeLocationId());
        detailVO.setTotalPrice(order.getTotalPrice());
        detailVO.setStatusEnum(order.getStatusEnum());
        detailVO.setApplyTime(order.getApplyTime());
        detailVO.setRemake(order.getRemake());
        // TODO: 关联查询供应商名称和目的库房名称并 set

        // 3. 查询并组装明细列表 (基于 bus_no 查询同一聚合根下的所有行)
        LambdaQueryWrapper<InStockOrder> itemWrapper = new LambdaQueryWrapper<>();
        itemWrapper.eq(InStockOrder::getBusNo, order.getBusNo());
        List<InStockOrder> itemEntities = inStockOrderMapper.selectList(itemWrapper);

        List<PurchaseInboundItemVO> itemVOs = itemEntities.stream().map(item -> {
            PurchaseInboundItemVO itemVO = new PurchaseInboundItemVO();
            itemVO.setId(item.getId());
            itemVO.setItemId(item.getItemId());
            itemVO.setLotNumber(item.getLotNumber());
            itemVO.setQuantity(item.getItemQuantity());
            itemVO.setUnit(item.getUnitCode());
            itemVO.setPrice(item.getPrice());
            itemVO.setSubTotal(item.getTotalPrice());
            // TODO: 关联查询药品名称、规格等并 set
            return itemVO;
        }).collect(Collectors.toList());

        detailVO.setItems(itemVOs);
        return detailVO;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void submitInboundOrder(PurchaseInboundSubmitDTO dto) {
        // 1. 校验单据是否存在
        InStockOrder order = inStockOrderMapper.selectById(dto.getId());
        if (order == null) {
            throw new RuntimeException("入库单不存在");
        }

        // 2. 状态校验：只有草稿(DRAFT)或已驳回(REJECTED)状态才能提交
        if (!"DRAFT".equals(order.getStatusEnum()) && !"REJECTED".equals(order.getStatusEnum())) {
            throw new RuntimeException("当前单据状态不允许提交审批");
        }

        // 3. 更新单据状态及提交时间
        order.setStatusEnum("PENDING_APPROVAL");
        order.setApplyTime(LocalDateTime.now());
        if (StringUtils.hasText(dto.getRemark())) {
            order.setRemake(dto.getRemark());
        }
        inStockOrderMapper.updateById(order);

        // 4. TODO: 如果前端传了新的明细，这里需要处理明细的增删改逻辑
    }

    /**
     * 私有方法：Entity 转 ListVO，并拼接流转路径
     */
    private PurchaseInboundListVO convertToListVO(InStockOrder order) {
        PurchaseInboundListVO vo = new PurchaseInboundListVO();
        vo.setId(order.getId());
        vo.setBusNo(order.getBusNo());
        vo.setStatusName(order.getStatusEnum()); // TODO: 建议转为中文枚举
        vo.setApplyTime(order.getApplyTime());
        vo.setTotalPrice(order.getTotalPrice());
        // TODO: 关联查询制单人姓名和审批人姓名

        // 核心逻辑：拼接流转路径 (西药库 → 门诊西药房)
        // TODO: 通过 sourceLocationId 和 purposeLocationId 查出对应的中文名称
        String sourceName = "未知来源"; // 替换为字典查询结果
        String purposeName = "未知目的"; // 替换为字典查询结果
        vo.setTransferPath(sourceName + " → " + purposeName);

        return vo;
    }
}