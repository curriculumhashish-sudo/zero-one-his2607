package com.zeroone.star.j7.pharmacy.service;

import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;

import java.util.List;

public interface PurchaseInboundService {

    /**
     * 获取入库单列表
     */
    List<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query);

    /**
     * 获取入库单详情
     */
    PurchaseInboundDetailVO getInboundDetail(Long id);
}