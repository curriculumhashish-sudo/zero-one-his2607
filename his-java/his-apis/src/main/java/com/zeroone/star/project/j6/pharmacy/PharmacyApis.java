package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.*;
import com.zeroone.star.project.dto.j6.pharmacy.InboundReceiptDeleteDTO;
import com.zeroone.star.project.query.j6.pharmacy.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.*;
import com.zeroone.star.project.dto.PageDTO;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;

public interface PharmacyApis {


    /**
     * 分页+多条件查询盘点单据列表
     * @param query 封装：分页参数 + 筛选条件（单据号、日期、药库、项目类型）
     * @return JsonVO包裹分页数据
     */
    @Operation(summary = "查询盘点单据列表")
    JsonVO<PharmacyInventoryQueryDTO> listInventory(PharmacyInventoryQuery query);

    /**
     * 根据id查询盘点单详情
     * @param query 封装单据id
     * @return JsonVO包裹完整单据详情（表头+明细）
     */
    @Operation(summary = "查询盘点单据详情")
    JsonVO<PharmacyInventoryDetailVO> getInventoryDetail(PharmacyInventoryDetailQuery query);

    /**
     * 删除单据
     * @param deleteDTO 前端提交的表单数据
     * @return JsonVO包裹新增后的单据基础信息
     */
    @Operation(summary = "删除单据")
    JsonVO<PharmacyInventoryDeleteDTO> deleteInventory(PharmacyInventoryDeleteDTO deleteDTO);



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

    /**
     * 获取库存明细列表(分页+条件)
     * 请求路径 /pageInventoryDetailCurrent
     * 请求方式 post
     * @param inventoryDetailQueryDTO 分页条件
     * @return 返回查询到的所有数据
     */
    JsonVO<InventoryDetailVO> pageInventoryDetailCurrent(InventoryDetailQuery inventoryDetailQueryDTO);


    /**
     * 停供/取消停供
     * 请求路径 /resumeInventoryItems
     * 请求方式 post
     * @param inventoryStatusDTO 需要停供/取消停供 的所有商品id
     * @return 成功修改了多少条数据
     */
    JsonVO<Integer> updateStatus(InventoryStatusDTO inventoryStatusDTO);


    /**
     * 导出数据
     * 请求路径 /exportInventoryDetailCurrent
     * 请求方式 post
     * @param inventoryDetailQueryDTO 条件 (根据条件导出excel)
     * @return 导出的数据
     */
    JsonVO<ExcelExportVO> exportInventoryDetailCurrent(InventoryDetailQuery inventoryDetailQueryDTO);



    /**
     * 展示表单全部信息
     * 展示出所有待审批或者正在审批或是已完成审批的表单信息
     * @param query 查询条件
     * @return 所有进货单列表
     */
    @GetMapping("/list")
    JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(@Valid InboundReceiptQuery query);

    /**
     * 展示选中表单的全部信息
     * 可以查看所有表单选中之后的列表
     * @param query 表单详情
     * @return 具体的表单数值
     */
    @GetMapping("/detail")
    JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(@Valid InboundReceiptDetailQuery query);

    /**
     * 删除表单
     * @param dto 选中表单的字段
     * @return 删除是否成功
     */
    @DeleteMapping("/remove")
    JsonVO<String> removeInboundReceipt(@Valid @RequestBody InboundReceiptDeleteDTO dto);

}
