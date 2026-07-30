package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;

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
}