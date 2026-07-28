//package com.zeroone.star.pharmacy.service.impl;
//
//
//import com.zeroone.star.pharmacy.service.IPharmacyInventoryService;
//
//import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDeleteVO;
//import lombok.RequiredArgsConstructor;
//import org.springframework.stereotype.Service;
//import org.springframework.transaction.annotation.Transactional;
//
//@Service
//@RequiredArgsConstructor
//public class PharmacyInventoryServiceImpl implements IPharmacyInventoryService {
//
//    //private final PharmacyInventoryMapper inventoryMapper;
//
//    @Override
//    @Transactional(rollbackFor = Exception.class)
//    public PharmacyInventoryDeleteVO removeInventory(String busNo) {
//        //1. 根据单据号查询单据（只查询必要字段，禁止select *）
//        PharmacyInventoryDO inventoryDO = inventoryMapper.selectByBusNo(busNo);
//
//        //2. 判断单据是否存在
//        if (inventoryDO == null) {
//            throw new RuntimeException("盘点单据【" + busNo + "】不存在");
//        }
//
//        //3. 业务状态校验：仅草稿允许删除
//        if (!"DRAFT".equals(inventoryDO.getStatusEnum())) {
//            throw new RuntimeException("仅草稿状态盘点单据可以删除，当前单据状态：" + inventoryDO.getStatusEnum());
//        }
//
//        //4. 执行删除
//        int deleteRow = inventoryMapper.deleteByBusNo(busNo);
//
//        //5. 组装返回VO
//        PharmacyInventoryDeleteVO resultVO = new PharmacyInventoryDeleteVO();
//        resultVO.setSuccess(deleteRow > 0);
//        return resultVO;
//    }
//}
