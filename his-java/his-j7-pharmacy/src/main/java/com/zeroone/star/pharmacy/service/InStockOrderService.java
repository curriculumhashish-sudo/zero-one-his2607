package com.zeroone.star.pharmacy.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDeleteDTO;
import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;
import com.zeroone.star.project.vo.j7.pharmacy.SupplyDocumentDeleteVO;

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
     */
    String create(InStockOrderDTO dto);

    /**
     * 保存入库单据（编辑草稿）
     */
    void update(String orderId, InStockOrderDTO dto);

    /**
     * 提交审批入库单据
     */
    void submit(String orderId, InStockOrderDTO dto);

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
