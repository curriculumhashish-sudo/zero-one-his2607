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

    /**
     * 获取单据列表（条件+分页）
     */
    IPage<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query);

    /**
     * 获取单据详情
     */
    PurchaseInboundDetailVO getInboundDetail(String id);

    /**
     * 提交审批
     */
    void submitInboundOrder(PurchaseInboundSubmitDTO dto);
}