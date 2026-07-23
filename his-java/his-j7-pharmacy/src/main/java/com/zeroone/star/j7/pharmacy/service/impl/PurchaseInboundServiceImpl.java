package com.zeroone.star.j7.pharmacy.service.impl;

import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;
import com.zeroone.star.j7.pharmacy.service.PurchaseInboundService;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class PurchaseInboundServiceImpl implements PurchaseInboundService {

    @Override
    public List<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query) {
        // TODO: 后续对接 Mapper 查询数据库，当前返回 null 作为假实现
        return null;
    }

    @Override
    public PurchaseInboundDetailVO getInboundDetail(Long id) {
        // TODO: 后续对接 Mapper 查询数据库，当前返回 null 作为假实现
        return null;
    }
}