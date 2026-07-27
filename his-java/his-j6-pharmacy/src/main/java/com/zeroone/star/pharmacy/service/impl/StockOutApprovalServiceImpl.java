package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.StockOutApprovalMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.service.IStockoutApprovalService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.WithdrawStockOutApprovalDTO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

/**
 * 药库出库审批 Service 实现
 */
@Service
public class StockOutApprovalServiceImpl implements IStockoutApprovalService {

    //
    private static final String TYPE_WAREHOUSE_STOCKOUT = "WAREHOUSE_STOCKOUT";
    //
    private static final String STATUS_DRAFT = "DRAFT";
    //待审批
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    //已撤回
    private static final String STATUS_WITHDRAW = "WITHDRAW";
    //已审批
    private static final String STATUS_AGREE = "AGREE";
    //被驳回
    private static final String STATUS_REJECT = "REJECT";

    @Resource
    private StockOutApprovalMapper stockOutApprovalMapper;

    @Resource
    private WkfInventoryItemMapper inventoryItemMapper;

    @Resource
    private UserHolder userHolder;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean submitStockOutApproval(SubmitStockOutApprovalDTO dto) {
        // 1. 查询出库单是否存在
        List<WkfSupplyRequest> rows = requireWarehouseStockOut(dto.getBusNo());
        // 2. 校验行状态一致
        requireSameStatus(rows);
        // 3. 校验当前状态允许提交审批（草稿/已撤回/已驳回）
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_DRAFT, STATUS_WITHDRAW, STATUS_REJECT);
        // 4. 更新状态为待审批
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        updateStatus(dto.getBusNo(), STATUS_PENDING_APPROVAL, wrapper -> wrapper
                .set("applicant_id", userId)
                .set("apply_time", now)
                .set("approver_id", dto.getApproverId())//指定审批人id
                .set("approval_time", null)
                .set("reason", null)
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    @Override
    @Transactional
    public Boolean withdrawStockOutApproval(WithdrawStockOutApprovalDTO dto) {
        //1. 查询出库订单是否存在
        List<WkfSupplyRequest> rows = requireWarehouseStockOut(dto.getBusNo());
        // 2. 校验行状态一致
        requireSameStatus(rows);
        // 3. 校验当前状态允许撤回审批（待审批）
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);
        //校验当前用户能否撤回该审批
        String userId = currentUserId();
        if(!rows.get(0).getApplicantId().equals(userId)){
            throw new IllegalStateException("当前用户不是该单据的申请人");
        }
        // 4. 更新状态为已撤回
        LocalDateTime now = LocalDateTime.now();
        updateStatus(dto.getBusNo(), STATUS_WITHDRAW, wrapper -> wrapper
                .set("approver_id",null)//清除审批人
                .set("approval_time",null)//清除审批时间
                .set("reason", null)
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean agreeStockOut(AgreeStockOutDTO dto) {
        // 1. 查询出库单是否存在
        List<WkfSupplyRequest> rows = requireWarehouseStockOut(dto.getBusNo());
        // 2. 校验行状态一致
        requireSameStatus(rows);
        // 3. 校验当前状态允许审批（待审批）
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);
        // 4. 校验当前用户是否为指定审批人
        String userId = currentUserId();
        if (!rows.get(0).getApproverId().equals(userId)) {
            throw new IllegalStateException("当前用户不是该单据的指定审批人");
        }
        // 5. 更新状态为已批准
        LocalDateTime now = LocalDateTime.now();
        updateStatus(dto.getBusNo(), STATUS_AGREE, wrapper -> wrapper
                .set("approver_id", userId)
                .set("approval_time", now)
                .set("updated_at", now)
                .set("updated_by", userId));

        // 6. 重新获取出库请求（状态更新后的最新数据）
        List<WkfSupplyRequest> approvedRows = requireWarehouseStockOut(dto.getBusNo());

        // 7. 创建出库配送记录(wkf_supply_delivery)
        createSupplyDelivery(approvedRows, dto.getContractBusNo(), now, userId);

        // 8. 源库库存扣减 + 目标药房库存增加(wkf_inventory_item)
        adjustInventoryForStockOut(approvedRows, now, userId);

        // 9. 生成药房入库申请单(wkf_supply_request)
        createPharmacyInboundRequest(approvedRows, dto.getContractBusNo(), now, userId);

        return true;
    }

    @Override
    @Transactional
    public Boolean rejectStockOut(RejectStockOutDTO dto) {
        // 1. 查询出库单是否存在
        List<WkfSupplyRequest> rows = requireWarehouseStockOut(dto.getBusNo());
        // 2. 校验行状态一致
        requireSameStatus(rows);
        // 3. 校验当前状态允许提交审批（待审批）
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);
        //  4. 校验当前用户是否为指定审批人
        String userId = currentUserId();
        if(!rows.get(0).getApproverId().equals(userId)){
            //当前用户不是单据的指定审批人
            throw new IllegalStateException("当前用户不是该单据的指定审批人");
        }
        // 5. 更新状态为被驳回(写入驳回原因)
        LocalDateTime now = LocalDateTime.now();
        updateStatus(dto.getBusNo(), STATUS_REJECT, wrapper -> wrapper
                .set("approver_id",userId)
                .set("approval_time",now)
                .set("reason", dto.getRejectReason())//写入驳回原因
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    // ==================== 私有方法 ====================

    /**
     * 查询药库出库单（按 busNo + type_enum + 未删除）
     */
    private List<WkfSupplyRequest> requireWarehouseStockOut(String busNo) {
        List<WkfSupplyRequest> rows = stockOutApprovalMapper.selectList(new QueryWrapper<WkfSupplyRequest>()
                .eq("bus_no", busNo)
                .eq("type_enum", TYPE_WAREHOUSE_STOCKOUT)
                .eq("is_deleted", 0));
        if (rows.isEmpty()) {
            throw new IllegalArgumentException("药库出库单不存在: " + busNo);
        }
        return rows;
    }

    /**
     * 校验同一单据下的所有行状态一致
     */
    private void requireSameStatus(List<WkfSupplyRequest> rows) {
        Set<String> statuses = rows.stream()
                .map(WkfSupplyRequest::getStatusEnum)
                .collect(Collectors.toSet());
        if (statuses.size() != 1) {
            throw new IllegalStateException("出库单行状态不一致: " + statuses);
        }
    }

    /**
     * 校验当前状态是否在允许范围内
     */
    private void requireAnyStatus(String currentStatus, String... allowedStatuses) {
        for (String allowedStatus : allowedStatuses) {
            if (allowedStatus.equals(currentStatus)) {
                return;
            }
        }
        throw new IllegalStateException("当前状态 " + currentStatus + " 不允许执行此操作");
    }

    /**
     * 批量更新出库单状态
     */
    private void updateStatus(String busNo, String status, StatusUpdateCustomizer customizer) {
        UpdateWrapper<WkfSupplyRequest> wrapper = new UpdateWrapper<WkfSupplyRequest>()
                .eq("bus_no", busNo)
                .eq("type_enum", TYPE_WAREHOUSE_STOCKOUT)
                .eq("is_deleted", 0)
                .set("status_enum", status);
        customizer.customize(wrapper);
        int affectedRows = stockOutApprovalMapper.update(null, wrapper);
        if (affectedRows <= 0) {
            throw new IllegalStateException("更新出库审批状态失败");
        }
    }

    /**
     * 获取当前用户ID
     */
    private String currentUserId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                return currentUser.getId();
            }
        } catch (Exception ignored) {
            // 无网关 header 时保持可用（本地测试等场景）
        }
        return "system";
    }

    /**
     * 创建出库配送记录（wkf_supply_delivery）
     * TODO: 待 wkf_supply_delivery 实体/Mapper 定义后实现
     */
    private void createSupplyDelivery(List<WkfSupplyRequest> rows, String contractBusNo,
                                       LocalDateTime now, String userId) {
        // TODO: 桩实现 — 需要 wkf_supply_delivery 实体
        // 遍历 rows，为每行创建一条配送记录，包含：
        //   - 出库单号(busNo)、物品信息(itemTable/itemId/unitCode/lotNumber)
        //   - 出库数量(itemQuantity)、源库位、目标库位(purposeLocationId)
        //   - 关联合同单号(contractBusNo)
    }

    /**
     * 源库库存扣减 + 目标药房库存增加
     */
    private void adjustInventoryForStockOut(List<WkfSupplyRequest> rows, LocalDateTime now, String userId) {
        for (WkfSupplyRequest row : rows) {
            BigDecimal quantity = row.getItemQuantity();
            if (quantity == null) {
                throw new IllegalStateException("出库数量不能为空，行ID: " + row.getId());
            }

            // 目标药房库存增加（purposeLocationId = 目标药房库位）
            UpdateWrapper<WkfInventoryItem> addWrapper = new UpdateWrapper<WkfInventoryItem>()
                    .eq("item_table", row.getItemTable())
                    .eq("item_id", row.getItemId())
                    .eq("location_id", row.getPurposeLocationId())
                    .eq("lot_number", row.getLotNumber())
                    .eq("unit_code", row.getUnitCode())
                    .eq("is_deleted", 0)
                    .setSql("quantity = quantity + " + quantity.toPlainString())
                    .set("updated_at", now)
                    .set("updated_by", userId);
            int affected = inventoryItemMapper.update(null, addWrapper);
            if (affected <= 0) {
                throw new IllegalStateException("目标药房库存记录不存在，locationId: "
                        + row.getPurposeLocationId() + ", itemId: " + row.getItemId());
            }

            // TODO: 源库库存扣减 — 需要出库源头表获取源库位(sourceLocationId)，暂时跳过
            // 实现方式：类似上方目标增加，将 .setSql("quantity = quantity - " + ...)
            // 条件中的 location_id 替换为源库位 ID
        }
    }

    /**
     * 生成药房入库申请单（wkf_supply_request，type_enum = WAREHOUSE_STOCKIN）
     */
    private void createPharmacyInboundRequest(List<WkfSupplyRequest> rows, String contractBusNo, LocalDateTime now, String userId) {
        // TODO: 桩实现 — type_enum 和入库单号生成规则待确认
        // 遍历 rows，向 wkf_supply_request 插入入库记录：
        //   - type_enum = "WAREHOUSE_STOCKIN"
        //   - status_enum = "DRAFT"
        //   - purposeLocationId 不变（目标药房）
        //   - busNo 使用新的入库单号（可从 contractBusNo 派生或自动生成）
    }

    @FunctionalInterface
    private interface StatusUpdateCustomizer {
        void customize(UpdateWrapper<WkfSupplyRequest> wrapper);
    }
}
