package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;

public interface InStockOrderService {
    Long create(InStockOrderDTO dto);
    void update(Long orderId, InStockOrderDTO dto);
    void submit(Long orderId, InStockOrderDTO dto);
}