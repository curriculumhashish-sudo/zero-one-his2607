package com.zeroone.star.pharmacy.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.InStockOrder;
import com.zeroone.star.pharmacy.entity.InStockOrderDetail;
import com.zeroone.star.pharmacy.mapper.InStockOrderDetailMapper;
import com.zeroone.star.pharmacy.mapper.InStockOrderMapper;
import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDetailDTO;
import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：入库单服务实现类
 * </p>
 * <p>
 * 功能说明：实现药房入库单的新增、草稿保存、保存并提交审批等核心业务功能。
 * 涉及的表包括：
 * - wkf_supply_request（供应请求主表）：充当入库单主表，管理状态与总金额。
 * - pharmacy_in_item（入库单明细表）：存储药品明细并参与汇总。
 * 在业务流程中，系统自动计算行合计金额和整单总金额，并严格控制状态流转
 * （草稿 DRAFT → 待审批 PENDING → 已审批 APPROVED → 入库完成 COMPLETED）。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Service
public class InStockOrderServiceImpl implements InStockOrderService {

    @Resource
    private InStockOrderMapper inStockOrderMapper;

    @Resource
    private InStockOrderDetailMapper detailMapper;

    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING = "PENDING";

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


    @Override
    @Transactional
    public String create(InStockOrderDTO dto) {
        InStockOrder order = buildOrder(dto);
        order.setStatusEnum(STATUS_DRAFT);
        order.setTotalPrice(BigDecimal.ZERO);
        inStockOrderMapper.insert(order);

        saveItems(order.getId(), dto.getItems());
        updateTotalPrice(order.getId());
        return order.getId();
    }

    @Override
    @Transactional
    public void update(String orderId, InStockOrderDTO dto) {
        InStockOrder existOrder = inStockOrderMapper.selectById(orderId);
        if (existOrder == null) {
            throw new RuntimeException("入库单不存在");
        }
        if (!STATUS_DRAFT.equals(existOrder.getStatusEnum())) {
            throw new RuntimeException("只有草稿状态才能编辑");
        }

        InStockOrder updateOrder = buildOrder(dto);
        updateOrder.setId(orderId);
        inStockOrderMapper.updateById(updateOrder);

        // 明细全量替换
        detailMapper.delete(new LambdaQueryWrapper<InStockOrderDetail>()
                .eq(InStockOrderDetail::getPharmacyId, orderId));
        saveItems(orderId, dto.getItems());

        updateTotalPrice(orderId);
    }

    @Override
    @Transactional
    public void submit(String orderId, InStockOrderDTO dto) {
        update(orderId, dto);

        int rows = inStockOrderMapper.update(null,
                new LambdaUpdateWrapper<InStockOrder>()
                        .eq(InStockOrder::getId, orderId)
                        .eq(InStockOrder::getStatusEnum, STATUS_DRAFT)
                        .set(InStockOrder::getStatusEnum, STATUS_PENDING));

        if (rows == 0) {
            throw new RuntimeException("当前状态不允许提交审批");
        }
    }

    // ==================== 私有方法 ====================

    private InStockOrder buildOrder(InStockOrderDTO dto) {
        InStockOrder order = new InStockOrder();
        order.setBusNo(dto.getOrderNo());
        order.setSupplierId(String.valueOf(dto.getSupplierId()));
        order.setOccurrenceTime(dto.getInDate() != null ?
                dto.getInDate().atStartOfDay() : LocalDateTime.now());
        order.setPurposeLocationId(String.valueOf(dto.getWarehouseId()));
        order.setRemake(dto.getRemark());
        // 设置入库类型和初始状态
        order.setTypeEnum("INVENTORY_IN");
        return order;
    }

    private void saveItems(String orderId, List<InStockOrderDetailDTO> itemDTOs) {
        List<InStockOrderDetail> items = new ArrayList<>();
        for (InStockOrderDetailDTO dto : itemDTOs) {
            InStockOrderDetail item = new InStockOrderDetail();
            item.setPharmacyId(orderId);
            item.setSeqNo(dto.getSeqNo());
            item.setDrugName(dto.getDrugName());
            item.setSpec(dto.getSpec());
            item.setManufacturer(dto.getManufacturer());
            item.setQuantity(dto.getQuantity());
            item.setUnitPrice(dto.getUnitPrice());
            item.setTotalAmount(dto.getUnitPrice().multiply(BigDecimal.valueOf(dto.getQuantity())));
            item.setBatchNo(dto.getBatchNo());
            item.setExpireDate(dto.getExpireDate());
            item.setItemRemark(dto.getItemRemark());
            items.add(item);
        }
        for (InStockOrderDetail item : items) {
            detailMapper.insert(item);
        }
    }

    private void updateTotalPrice(String orderId) {
        List<InStockOrderDetail> items = detailMapper.selectList(
                new LambdaQueryWrapper<InStockOrderDetail>()
                        .eq(InStockOrderDetail::getPharmacyId, orderId));
        BigDecimal total = items.stream()
                .map(InStockOrderDetail::getTotalAmount)
                .reduce(BigDecimal.ZERO, BigDecimal::add);

        InStockOrder order = new InStockOrder();
        order.setId(orderId);
        order.setTotalPrice(total);
        inStockOrderMapper.updateById(order);
    }
}