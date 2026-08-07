package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.stocktake.*;
import com.zeroone.star.project.query.j6.pharmacy.PharmacyInventoryDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.stocktake.PharmacyInventoryQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryListVO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.StocktakeDetailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import org.springframework.web.bind.annotation.PathVariable;

public interface WarehouseStocktakeApis {

    /**
     * 分页+多条件查询盘点单据列表
     */
    JsonVO<PharmacyInventoryListVO> listInventory(PharmacyInventoryQuery query);

    /**
     * 根据id查询盘点单详情
     */
    JsonVO<PharmacyInventoryDetailVO> getInventoryDetail(PharmacyInventoryDetailQuery query);

    /**
     * 删除盘点单据
     */
    JsonVO<Boolean> deleteStocktake(@Parameter(description = "盘点单号") String busNo);

    /**
     * 新增盘点单。
     * 发送简洁上下文，返回空草稿的完整文档结构。
     * @param dto 新增上下文（库房、盘点人等）
     * @return 完整的空白文档结构
     */
    @Operation(summary = "新增盘点单", description = "生成空盘点单，状态=草稿")
    JsonVO<StocktakeDetailVO> createStocktake(StocktakeAddDTO dto);

    /**
     * 保存盘点单。
     * 请求参数与新增返回响应结构一致。
     * @param busNo 盘点单号
     * @param dto   保存数据
     * @return 更新后的盘点单详情
     */
    @Operation(summary = "保存盘点单", description = "填入实盘数据 + 快照锁定账面数，状态=草稿")
    JsonVO<StocktakeDetailVO> saveStocktake(
            @PathVariable @Parameter(description = "盘点单号", required = true) String busNo,
            StocktakeSaveRequestDTO dto);

    /**
     * 保存盘点单并提交审批。
     * 请求参数与新增返回响应结构一致。
     * @param busNo 盘点单号
     * @param dto   保存并提交数据
     * @return 更新后的盘点单详情（状态=PENDING_APPROVAL）
     */
    @Operation(summary = "保存并提交审批", description = "填入实盘数据，状态草稿→待审批")
    JsonVO<StocktakeDetailVO> submitStocktake(
            @PathVariable @Parameter(description = "盘点单号", required = true) String busNo,
            StocktakeSaveRequestDTO dto);

    /**
     * 提交药库盘点审批
     */
    JsonVO<Boolean> submitStocktakeApproval(StocktakeSubmitApprovalDTO dto);

    /**
     * 撤回药库盘点审批
     */
    JsonVO<Boolean> withdrawStocktakeApproval(StocktakeWithdrawApprovalDTO dto);

    /**
     * 通过药库盘点审批
     */
    JsonVO<Boolean> agreeStocktakeApproval(StocktakeAgreeApprovalDTO dto);

    /**
     * 驳回药库盘点审批
     */
    JsonVO<Boolean> rejectStocktakeApproval(StocktakeRejectApprovalDTO dto);
}
