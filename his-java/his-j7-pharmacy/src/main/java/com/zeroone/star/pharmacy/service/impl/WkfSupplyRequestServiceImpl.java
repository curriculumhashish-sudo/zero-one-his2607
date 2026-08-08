package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.entity.DocInventoryItemSnapshot;
import com.zeroone.star.pharmacy.entity.InventoryLock;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.DocInventoryItemSnapshotMapper;
import com.zeroone.star.pharmacy.mapper.InventoryLockMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.vo.j7.pharmacy.StocktakeStatusVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.Iterator;
import java.util.List;

/**
 * <p>
 * 工作流 supply request 服务实现类
 * </p>
 *
 * @author tsfmn
 * @since 2026-07-22
 */
@Service
@Slf4j
public class WkfSupplyRequestServiceImpl extends ServiceImpl<WkfSupplyRequestMapper, WkfSupplyRequest> implements IWkfSupplyRequestService {

    @Resource
    WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Resource
    WkfInventoryItemMapper wkfInventoryItemMapper;

    @Resource
    InventoryLockMapper inventoryLockMapper;

    @Resource
    DocInventoryItemSnapshotMapper docInventoryItemSnapshotMapper;

    /**
     * 提交审批业务流程
     * @param dto
     * @return
     */
    @Override
    //后续换成seata
    @Transactional(rollbackFor = Exception.class)
    public StocktakeStatusVO submit(OperatorDTO dto) {
        if (dto == null) {
            log.error("DTO不存在");
            throw new IllegalArgumentException("DTO不存在");
        }
        //先查询出盘点单号下的所有药物明细
        List<WkfSupplyRequest> list = wkfSupplyRequestMapper.getPharmacyStockTakeList(dto.getBusNo());
        if (list == null || list.isEmpty()){
            log.error("盘点单明细中没有数据: {}", dto.getBusNo());
            throw new IllegalArgumentException("盘点单明细中没有数据: " + dto.getBusNo());
        }
        //校验审批状态
        Iterator<WkfSupplyRequest> iterator = list.stream().iterator();
        while (iterator.hasNext()){
            WkfSupplyRequest next = iterator.next();
            if (!"DRAFT".equals(next.getStatusEnum())) {
                log.error("盘点单明细存在非草稿状态: {}", next.getId());
                throw new IllegalArgumentException("盘点单明细存在非草稿状态: " + next.getId());
            }
        }

        //修改审批状态
        wkfSupplyRequestMapper.updateStatusForSubmit("PENDING_APPROVAL", dto.getOperatorId(), LocalDateTime.now(), dto.getBusNo());
        log.info("审批状态修改成功: {}", dto.getBusNo());

        //2.查询库存记录与盘点单但情况，加锁，冻结快照
        int orderFlag = 1;
        for (WkfSupplyRequest request : list){

            // 1. 查找库存记录（三元组唯一定位）
            WkfInventoryItem item = wkfInventoryItemMapper.findByItemLotAndLocation(
                    request.getItemId(),
                    request.getLotNumber(),
                    request.getPurposeLocationId());
            if (item == null){
                log.error("盘点单明细中没有对应的库存记录: {}", request.getId());
                throw new IllegalArgumentException("盘点单明细中没有对应的库存记录: " + request.getId());
            }

            //2.检查其他盘点单是否有锁住
            List<InventoryLock> lockList = inventoryLockMapper
                    .findByInventoryId(item.getId());
            for (InventoryLock lock : lockList){
                if (!lock.getBusNo().equals(request.getBusNo())){
                    log.error("库存项已被其他盘点单锁定: inventoryId={}, busNo={}",
                            item.getId(), lock.getBusNo());
                    throw new IllegalStateException("库存项 " + item.getId()
                            + " 已被盘点单 " + lock.getBusNo() + " 锁定");
                }
            }

            // 3. 加锁
            InventoryLock lock = new InventoryLock();
            lock.setTenantId(item.getTenantId());
            lock.setInventoryId(item.getId());
            lock.setLockFlag(true);
            lock.setStatus("LOCKED");
            lock.setBusNo(request.getBusNo());
            lock.setOrderFlag(orderFlag++);
            lock.setCreatedAt(LocalDateTime.now());
            lock.setCreatedBy(dto.getOperatorId());
            inventoryLockMapper.insert(lock);

            // 4. 冻结快照（记录此刻的账面库存数量）
            DocInventoryItemSnapshot snapshot = new DocInventoryItemSnapshot();
            snapshot.setTenantId(item.getTenantId());
            snapshot.setBusNo(request.getBusNo());
            snapshot.setSnapshotTime(LocalDateTime.now());
            snapshot.setInventoryItemId(item.getId());
            snapshot.setItemTable(item.getItemTable());
            snapshot.setItemId(item.getItemId());
            snapshot.setWarehouseId(item.getLocationId());
            snapshot.setSupplierId(item.getSupplierId());
            snapshot.setName(item.getName());
            snapshot.setLotNumber(item.getLotNumber());
            snapshot.setQuantity(item.getQuantity());       // 冻结的账面数量
            snapshot.setPackageUnitCode(item.getUnitCode());
            snapshot.setProductionDate(item.getProductionDate());
            snapshot.setExpirationDate(item.getExpirationDate());
            snapshot.setInventoryStatusEnum(item.getInventoryStatusEnum());
            snapshot.setTraceNo(item.getTraceNo());
            snapshot.setCreatedBy(dto.getOperatorId());
            snapshot.setCreatedAt(LocalDateTime.now());
            docInventoryItemSnapshotMapper.insert(snapshot);


        }
        // 构造返回值
        WkfSupplyRequest first = list.get(0);
        StocktakeStatusVO vo = new StocktakeStatusVO();
        vo.setBusNo(first.getBusNo());
        vo.setTypeEnum(first.getTypeEnum());
        vo.setStatusEnum("PENDING_APPROVAL");
        vo.setCreatedBy(first.getCreatedBy());
        vo.setCreatedAt(first.getCreatedAt());
        vo.setApplicantId(dto.getOperatorId());
        vo.setApplyTime(LocalDateTime.now());

        log.info("盘点单提交审批成功: {}", dto.getBusNo());
        return vo;
    }

    /**
     * 审批通过业务流程
     * @param dto
     * @return
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeStatusVO approve(ApproveDTO dto) {
        //1.校验
        List<WkfSupplyRequest> pharmacyStockTakeList = wkfSupplyRequestMapper.getPharmacyStockTakeList(dto.getBusNo());
        if (pharmacyStockTakeList == null || pharmacyStockTakeList.isEmpty()) {
            log.error("盘点单明细中没有数据: {}", dto.getBusNo());
            throw new IllegalArgumentException("盘点单明细中没有数据: " + dto.getBusNo());
        }
        for (WkfSupplyRequest request : pharmacyStockTakeList) {
            if (!"PENDING_APPROVAL".equals(request.getStatusEnum())) {
                log.error("盘点单明细状态异常: {}", request.getId());
                throw new IllegalArgumentException("盘点单明细状态异常，不是待审批状态: " + request.getId());
            }
        }

        LocalDateTime approvalTime = dto.getApprovalTime() == null ? LocalDateTime.now() : dto.getApprovalTime();

        //2.改状态
        wkfSupplyRequestMapper.updateStatusForApprove("AGREE", dto.getApproverId(), approvalTime, dto.getBusNo());
        log.info("审批状态修改成功: {}", dto.getBusNo());

        //3.逐行更新库存
        for (WkfSupplyRequest request : pharmacyStockTakeList) {

            //平账直接跳过
            if (request.getItemQuantity().compareTo(BigDecimal.ZERO) == 0) {
                continue;
            }

            //找到对应库存项
            WkfInventoryItem item = wkfInventoryItemMapper.findByItemLotAndLocation(
                    request.getItemId(),
                    request.getLotNumber(),
                    request.getPurposeLocationId());

            if (item == null) {
                log.error("库存项不存在: itemId={}, lotNumber={}, locationId={}",
                        request.getItemId(), request.getLotNumber(), request.getPurposeLocationId());
                throw new IllegalStateException("库存项不存在: " + request.getItemId());
            }

            // 更新库存（乐观锁防并发）
            int affected = wkfInventoryItemMapper.updateQuantity(
                    item.getId(),
                    request.getItemQuantity(),
                    item.getVersion(),
                    dto.getApproverId());

            if (affected == 0) {
                log.error("库存项版本冲突: {}", item.getId());
                throw new IllegalStateException("库存项版本冲突: " + item.getId());
            }
        }

        //4.释放锁
        inventoryLockMapper.deleteByBusNo(dto.getBusNo());
        log.info("库存锁释放成功: {}", dto.getBusNo());

        //5.构造返回值
        WkfSupplyRequest first = pharmacyStockTakeList.get(0);
        StocktakeStatusVO vo = new StocktakeStatusVO();
        vo.setBusNo(first.getBusNo());
        vo.setTypeEnum(first.getTypeEnum());
        vo.setStatusEnum("AGREE");
        vo.setCreatedBy(first.getCreatedBy());
        vo.setCreatedAt(first.getCreatedAt());
        vo.setApplicantId(first.getApplicantId());
        vo.setApplyTime(first.getApplyTime());
        vo.setApproverId(dto.getApproverId());
        vo.setApprovalTime(approvalTime);

        log.info("盘点单审批通过: {}", dto.getBusNo());
        return vo;
    }

    /**
     * 审批驳回业务流程
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeStatusVO reject(ApproveDTO dto) {
        // 1. 校验
        List<WkfSupplyRequest> list = wkfSupplyRequestMapper.getPharmacyStockTakeList(dto.getBusNo());
        if (list == null || list.isEmpty()) {
            log.error("盘点单明细中没有数据: {}", dto.getBusNo());
            throw new IllegalArgumentException("盘点单明细中没有数据: " + dto.getBusNo());
        }
        for (WkfSupplyRequest request : list) {
            if (!"PENDING_APPROVAL".equals(request.getStatusEnum())) {
                log.error("盘点单明细状态异常: {}", request.getId());
                throw new IllegalArgumentException("盘点单明细状态异常，不是待审批状态: " + request.getId());
            }
        }

        LocalDateTime approvalTime = dto.getApprovalTime() == null ? LocalDateTime.now() : dto.getApprovalTime();

        // 2. 改状态 → REJECT
        wkfSupplyRequestMapper.updateStatusForApprove("REJECT", dto.getApproverId(), approvalTime, dto.getBusNo());
        log.info("审批状态修改为驳回: {}", dto.getBusNo());

        // 3. 释放锁（库存不动）
        inventoryLockMapper.deleteByBusNo(dto.getBusNo());
        log.info("库存锁释放成功: {}", dto.getBusNo());

        // 4. 返回
        WkfSupplyRequest first = list.get(0);
        StocktakeStatusVO vo = new StocktakeStatusVO();
        vo.setBusNo(first.getBusNo());
        vo.setTypeEnum(first.getTypeEnum());
        vo.setStatusEnum("REJECT");
        vo.setCreatedBy(first.getCreatedBy());
        vo.setCreatedAt(first.getCreatedAt());
        vo.setApplicantId(first.getApplicantId());
        vo.setApplyTime(first.getApplyTime());
        vo.setApproverId(dto.getApproverId());
        vo.setApprovalTime(approvalTime);

        log.info("盘点单审批驳回: {}", dto.getBusNo());
        return vo;
    }

    /**
     * 撤回审批业务流程
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeStatusVO withdraw(OperatorDTO dto) {
        // 1. 校验
        List<WkfSupplyRequest> list = wkfSupplyRequestMapper.getPharmacyStockTakeList(dto.getBusNo());
        if (list == null || list.isEmpty()) {
            log.error("盘点单明细中没有数据: {}", dto.getBusNo());
            throw new IllegalArgumentException("盘点单明细中没有数据: " + dto.getBusNo());
        }
        for (WkfSupplyRequest request : list) {
            if (!"PENDING_APPROVAL".equals(request.getStatusEnum())) {
                log.error("盘点单明细状态异常: {}", request.getId());
                throw new IllegalArgumentException("盘点单明细状态异常，不是待审批状态: " + request.getId());
            }
        }

        // 2. 改状态 → DRAFT，清空申请人信息
        wkfSupplyRequestMapper.updateStatusForWithdraw("DRAFT", dto.getBusNo());
        log.info("审批状态回退为草稿: {}", dto.getBusNo());

        // 3. 释放锁
        inventoryLockMapper.deleteByBusNo(dto.getBusNo());
        log.info("库存锁释放成功: {}", dto.getBusNo());

        // 4. 返回
        WkfSupplyRequest first = list.get(0);
        StocktakeStatusVO vo = new StocktakeStatusVO();
        vo.setBusNo(first.getBusNo());
        vo.setTypeEnum(first.getTypeEnum());
        vo.setStatusEnum("DRAFT");
        vo.setCreatedBy(first.getCreatedBy());
        vo.setCreatedAt(first.getCreatedAt());

        log.info("盘点单撤回成功: {}", dto.getBusNo());
        return vo;
    }

}
