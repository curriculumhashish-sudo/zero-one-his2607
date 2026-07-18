package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSaveDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.StocktakeDetailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;

@Tag(name = "药库管理")
public interface PharmacyApis {

    /**
     * 新增盘点单
     * <p>服务端根据头信息生成空明细行（占位），状态标记为草稿</p>
     *
     * @param dto 头信息（purposeLocationId、categoryEnum 必填，busNo 为空）
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
     * @param dto   头信息 + 明细行列表
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
     * @param dto   头信息 + 明细行列表
     * @return 更新后的盘点单详情（状态=PENDING_APPROVAL）
     */
    @Operation(summary = "保存盘点单并提交审批", description = "填入实盘数据，状态草稿→待审批")
    @PutMapping("/pharmacy/stocktake/{busNo}/submit")
    JsonVO<StocktakeDetailVO> submitStocktake(
            @Parameter(description = "盘点单号", required = true, example = "PD202607180001")
            @PathVariable String busNo,
            @RequestBody StocktakeSaveDTO dto
    );
}
