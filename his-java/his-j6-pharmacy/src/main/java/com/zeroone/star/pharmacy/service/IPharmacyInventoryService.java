package com.zeroone.star.pharmacy.service;


import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDeleteVO;

public interface IPharmacyInventoryService {
    /**
     * 删除盘点单据
     * @param busNo 单据号
     * @return 删除结果VO
     */
    PharmacyInventoryDeleteVO removeInventory(String busNo);
}
