package com.zeroone.star.pharmacy.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j7.pharmacy.*;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;
import com.zeroone.star.project.vo.j7.pharmacy.SupplyDocumentDeleteVO;
import org.springframework.transaction.annotation.Transactional;

/**
 * <p>
 * 描述：药房入库单服务接口
 * </p>
 * <p>
 * 功能说明：提供入库单的新增、草稿保存、保存并提交审批等功能。
 * 所有操作均在事务中执行，并同步维护明细数据及总金额。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
public interface InStockOrderService {
    /**
     * 新增入库单据
     * @return 包含完整信息的入库单（基础字段 + 单据状态/制单人/审批人/审批时间/总金额）
     */
    InStockOrderSaveDTO create(InStockOrderCreateDTO dto);

    /**
     * 保存入库单据（编辑草稿）
     */
    void update(String orderId, InStockOrderSaveDTO dto);

    /**
     * 提交审批入库单据（纯状态流转，仅需单据号）
     */
    void submitByOrderId(String orderId);

//    default IPage<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query) {
//        throw new UnsupportedOperationException("入库单列表查询接口暂未实现");
//    }
//
//    default PurchaseInboundDetailVO getInboundDetail(String id) {
//        throw new UnsupportedOperationException("入库单详情查询接口暂未实现");
//    }
//
//    default void submitInboundOrder(PurchaseInboundSubmitDTO dto) {
//        throw new UnsupportedOperationException("入库单提交审批接口暂未实现");
//    }

    /**
     * 保存并提交审批
     */
    void submit(String orderId, InStockOrderSaveDTO dto);

//    default IPage<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query) {
//        throw new UnsupportedOperationException("入库单列表查询接口暂未实现");
//    }
//
//    default PurchaseInboundDetailVO getInboundDetail(String id) {
//        throw new UnsupportedOperationException("入库单详情查询接口暂未实现");
//    }
//
//    default void submitInboundOrder(PurchaseInboundSubmitDTO dto) {
//        throw new UnsupportedOperationException("入库单提交审批接口暂未实现");
//    }

    /**
     * <p>
     * 获取单据列表（条件+分页）
     * </p>
     *
     * @author chunmu
     */
    IPage<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query);

    /**
     * <p>
     * 获取单据详情
     * </p>
     *
     * @author chunmu
     */
    PurchaseInboundDetailVO getInboundDetail(String id);

    /**
     * <p>
     * 保存并提交审批
     * </p>
     *
     * @author chunmu
     */
    void submitInboundOrder(PurchaseInboundSubmitDTO dto);

    /**
     * 删除入库单据
     * <p>
     * 仅 DRAFT（草稿）或 PENDING_APPROVAL（待审核）状态可删除。
     * 软删除 wkf_supply_request 主表，硬删 pharmacy_in_item 明细，解锁 inventory_lock。
     * </p>
     *
     * @param dto 删除请求（含单据号和原因）
     * @return 删除结果
     */
    SupplyDocumentDeleteVO deleteOrder(InStockOrderDeleteDTO dto);
}
