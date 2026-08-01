package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.*;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutSaveResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

public interface WarehouseStockOutApis {

    @PostMapping("/save-and-submit")
    @Operation(summary = "保存并提交审批")
    JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(@Valid @RequestBody StockOutSaveDTO dto);

    @PostMapping("/add")
    @Operation(summary = "新增单据")
    JsonVO<StockOutSaveResultVO> addStockOut(@Valid @RequestBody StockOutSaveDTO dto);

    /**
     * 保存出库单（新增/编辑）
     *
     * <p>支持新增和编辑两种场景：
     * <ul>
     *   <li>新增：busNo为空，系统自动生成单据号，状态为草稿</li>
     *   <li>编辑：busNo不为空，根据单据号更新数据</li>
     * </ul>
     * </p>
     *
     * @param dto 出库单数据
     * @return 保存结果（含单据号和状态）
     */
    @Operation(summary = "保存出库单", description = "新增或编辑出库单")
    JsonVO<StockOutSaveResultVO> saveStockOut(
            @Parameter(description = "出库单数据", required = true) StockOutSaveDTO dto);

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
    JsonVO<StockOutApprovalResultVO> submitStockOutApproval(SubmitStockOutApprovalDTO dto);

    /**
     * 撤回出库审批
     * @param dto 撤回审批参数（busNo）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<StockOutApprovalResultVO> withdrawStockOutApproval(WithdrawStockOutApprovalDTO dto);

    /**
     * 通过出库审批
     * @param dto 通过审批参数（busNo + 可选contractBusNo）
     * @return 操作结果（含操作后状态 + 药房入库壳单号）
     */
    JsonVO<StockOutApprovalResultVO> agreeStockOut(AgreeStockOutDTO dto);

    /**
     * 驳回出库审批
     * @param dto 驳回审批参数（busNo + rejectReason）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<StockOutApprovalResultVO> rejectStockOut(RejectStockOutDTO dto);
}
