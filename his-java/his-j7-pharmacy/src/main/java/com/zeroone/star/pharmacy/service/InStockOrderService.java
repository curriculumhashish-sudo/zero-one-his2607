package com.zeroone.star.pharmacy.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;
import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;

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
    String create(InStockOrderDTO dto);
    void update(String orderId, InStockOrderDTO dto);
    void submit(String orderId, InStockOrderDTO dto);

    default IPage<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query) {
        throw new UnsupportedOperationException("入库单列表查询接口暂未实现");
    }

    default PurchaseInboundDetailVO getInboundDetail(String id) {
        throw new UnsupportedOperationException("入库单详情查询接口暂未实现");
    }

    default void submitInboundOrder(PurchaseInboundSubmitDTO dto) {
        throw new UnsupportedOperationException("入库单提交审批接口暂未实现");
    }
}
