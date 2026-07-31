package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptDeleteDTO;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptQuery;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptListVO;

import java.util.List;

/**
 * 入库单业务服务接口（列表 / 详情 / 删除）
 *
 * @author BMXB
 * @since 2026-07-28
 */
public interface IInboundReceiptService {

    /**
     * 条件分页查询入库单列表
     */
    List<InboundReceiptListVO> queryAllInboundReceipt(InboundReceiptQuery query);

    /**
     * 查询入库单详情（头 + 明细行）
     */
    InboundReceiptDetailVO getInboundReceiptDetail(InboundReceiptDetailQuery query);

    /**
     * 删除入库单（逻辑删除）
     */
    String removeInboundReceipt(InboundReceiptDeleteDTO dto);
}
