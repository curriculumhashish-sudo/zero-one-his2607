package com.zeroone.star.pharmacy.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;

/**
 * 药房入库单 Service 接口
 */
public interface InStockOrderService {

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