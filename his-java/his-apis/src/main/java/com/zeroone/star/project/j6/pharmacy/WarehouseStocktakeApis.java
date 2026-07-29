package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.stocktake.*;
import com.zeroone.star.project.query.j6.pharmacy.PharmacyInventoryDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.stocktake.PharmacyInventoryQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryListVO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.PharmacyInventoryVO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.StocktakeDetailVO;
import io.swagger.v3.oas.annotations.Parameter;

public interface WarehouseStocktakeApis {

    /**
     * 分页+多条件查询盘点单据列表
     * @param query 封装：分页参数 + 筛选条件（单据号、日期、药库、项目类型）
     * @return JsonVO包裹分页数据
     */
    JsonVO<PharmacyInventoryListVO> listInventory(PharmacyInventoryQuery query);

    /**
     * 根据id查询盘点单详情
     * @param query 封装单据id
     * @return JsonVO包裹完整单据详情（表头+明细）
     */
    JsonVO<PharmacyInventoryDetailVO> getInventoryDetail(PharmacyInventoryDetailQuery query);

    /**
     * 删除盘点单据
     * @param busNo
     * @return JsonVO包裹删除后的单据基础信息
     */

    JsonVO<Boolean> deleteStocktake(@Parameter(description = "盘点单号") String busNo);



    /**
     * 新增盘点单
     * <p>服务端根据头信息生成空明细行（占位），状态标记为草稿</p>
     *
     * @param dto 头信息（purposeLocationId、categoryEnum 必填，busNo 由服务端生成，无需传入）
     * @return 含空明细行的盘点单详情
     */
    JsonVO<StocktakeDetailVO> createStocktake(@Parameter StocktakeSaveDTO dto);

    /**
     * 保存盘点单
     * <p>填入完整实盘数据，快照锁定当前账面数（通过 inventoryItemId 关联 wkf_inventory_item），状态保持草稿</p>
     *
     * @param busNo 盘点单号（路径参数）
     * @param dto   头信息 + 明细行列表(明细行仅含 inventoryItemId, traceNo, remake, actualQuantity, profitLossQuantity)
     * @return 更新后的盘点单详情
     */
    JsonVO<StocktakeDetailVO> saveStocktake(
            @Parameter(description = "盘点单号", required = true, example = "PD202607180001")
            String busNo, @Parameter StocktakeSaveDTO dto
    );

    /**
     * 保存盘点单并提交审批
     * <p>将盘点明细数据更新到库中，获取申请人和申请时间，将状态从草稿更新为待审批</p>
     *
     * @param busNo 盘点单号（路径参数）
     * @param dto   头信息 + 明细行列表(明细行仅含 inventoryItemId, traceNo, remake, actualQuantity, profitLossQuantity)
     * @return 更新后的盘点单详情（状态=PENDING_APPROVAL）
     */
    JsonVO<StocktakeDetailVO> submitStocktake(
            @Parameter(description = "盘点单号", required = true, example = "PD202607180001")
            String busNo, @Parameter StocktakeSaveDTO dto
    );

    /**
     * 提交药库盘点审批
     *
     * @param dto 提交审批参数
     * @return JsonVO<Boolean> 提交结果
     */
    JsonVO<Boolean> submitStocktakeApproval(StocktakeSubmitApprovalDTO dto);

    /**
     * 撤回药库盘点审批
     *
     * @param dto 撤回审批参数
     * @return JsonVO<Boolean> 撤回结果
     */
    JsonVO<Boolean> withdrawStocktakeApproval(StocktakeWithdrawApprovalDTO dto);

    /**
     * 通过药库盘点审批
     *
     * @param dto 审批通过参数
     * @return JsonVO<Boolean> 审批结果
     */
    JsonVO<Boolean> agreeStocktakeApproval(StocktakeAgreeApprovalDTO dto);

    /**
     * 驳回药库盘点审批
     *
     * @param dto 审批驳回参数
     * @return JsonVO<Boolean> 驳回结果
     */
    JsonVO<Boolean> rejectStocktakeApproval(StocktakeRejectApprovalDTO dto);
}
