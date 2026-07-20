package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.RejectInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.SubmitInboundReceiptApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.WithdrawInboundReceiptApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundApprovalResultVO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.StocktakeDetailVO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j6.pharmacy.StockOutQuery;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutListVO;
import com.zeroone.star.project.dto.j6.pharmacy.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.WithdrawStockOutApprovalDTO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;

public interface PharmacyApis {

    /**
     * 新增盘点单
     * <p>服务端根据头信息生成空明细行（占位），状态标记为草稿</p>
     *
     * @param dto 头信息（purposeLocationId、categoryEnum 必填，busNo 由服务端生成，无需传入）
     * @return 含空明细行的盘点单详情
     */
    @Operation(summary = "新增盘点单", description = "生成空盘点单, 状态=草稿")
    @PostMapping("/pharmacy/stocktake/create")
    JsonVO<StocktakeDetailVO> createStocktake(@RequestBody StocktakeSaveDTO dto);

    /**
     * 保存盘点单
     * <p>填入完整实盘数据，快照锁定当前账面数（通过 inventoryItemId 关联 wkf_inventory_item），状态保持草稿</p>
     *
     * @param busNo 盘点单号（路径参数）
     * @param dto   头信息 + 明细行列表(明细行仅含 inventoryItemId, traceNo, remake, actualQuantity, profitLossQuantity)
     * @return 更新后的盘点单详情
     */
    @Operation(summary = "保存盘点单", description = "填入实盘数据 + 快照锁定账面数，状态=草稿")
    @PutMapping("/pharmacy/stocktake/{busNo}/save")
    JsonVO<StocktakeDetailVO> saveStocktake(
            @Parameter(description = "盘点单号", required = true, example = "PD202607180001")
            @PathVariable String busNo,
            @RequestBody StocktakeSaveDTO dto
    );

    /**
     * 保存盘点单并提交审批
     * <p>将盘点明细数据更新到库中，获取申请人和申请时间，将状态从草稿更新为待审批</p>
     *
     * @param busNo 盘点单号（路径参数）
     * @param dto   头信息 + 明细行列表(明细行仅含 inventoryItemId, traceNo, remake, actualQuantity, profitLossQuantity)
     * @return 更新后的盘点单详情（状态=PENDING_APPROVAL）
     */
    @Operation(summary = "保存盘点单并提交审批", description = "填入实盘数据，状态草稿→待审批")
    @PutMapping("/pharmacy/stocktake/{busNo}/submit")
    JsonVO<StocktakeDetailVO> submitStocktake(
            @Parameter(description = "盘点单号", required = true, example = "PD202607180001")
            @PathVariable String busNo,
            @RequestBody StocktakeSaveDTO dto
    );


    /**
     * 提交审批：DRAFT → PENDING_APPROVAL
     */
    JsonVO<InboundApprovalResultVO> submitInboundReceiptApproval(SubmitInboundReceiptApprovalDTO dto);

    /**
     * 撤回审批：PENDING_APPROVAL → DRAFT
     */
    JsonVO<InboundApprovalResultVO> withdrawInboundReceiptApproval(WithdrawInboundReceiptApprovalDTO dto);

    /**
     * 审批通过：PENDING_APPROVAL → AGREE
     */
    JsonVO<InboundApprovalResultVO> agreeInboundReceipt(AgreeInboundReceiptDTO dto);

    /**
     * 驳回审批：PENDING_APPROVAL → REJECT
     */
    JsonVO<InboundApprovalResultVO> rejectInboundReceipt(RejectInboundReceiptDTO dto);


    /**
     * 删除出库单
     * @param id 出库单ID
     * @return 是否成功
     */
    JsonVO<String> removeStockOut(String id);

    /**
     * 获取出库单详情
     * @param id 出库单ID
     * @return 详情
     */
    JsonVO<StockOutDetailVO> getStockOutDetail(String id);

    /**
     * 分页、条件查询出库单列表
     * @param stockOutQuery 查询条件
     * @return 分页结果
     */
    JsonVO<PageDTO<StockOutListVO>> getPharmacy(StockOutQuery stockOutQuery);

    /**
     * 提交出库审批
     * @param dto 提交审批参数（busNo）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<String> submitStockOutApproval(SubmitStockOutApprovalDTO dto);

    /**
     * 撤回出库审批
     * @param dto 撤回审批参数（busNo）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<String> withdrawStockOutApproval(WithdrawStockOutApprovalDTO dto);

    /**
     * 通过出库审批
     * @param dto 通过审批参数（busNo + 可选contractBusNo）
     * @return 操作结果（含操作后状态 + 药房入库壳单号）
     */
    JsonVO<String> agreeStockOut(AgreeStockOutDTO dto);

    /**
     * 驳回出库审批
     * @param dto 驳回审批参数（busNo + rejectReason）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<String> rejectStockOut(RejectStockOutDTO dto);
}
