package com.zeroone.star.project.j7.stocktake;

import com.zeroone.star.project.dto.j7.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.stocktake.StocktakeDraftVO;
import com.zeroone.star.project.vo.j7.stocktake.StocktakeSaveResultVO;
import com.zeroone.star.project.vo.j7.stocktake.StocktakeSubmitResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;

/**
 * <p>
 * 描述：盘点单新增保存相关接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Tag(name = "盘点单新增保存", description = "新增单据、保存单据、保存并提交审批接口")
public interface StocktakeApis {

    /**
     * 新增盘点单草稿。
     * @return 草稿默认值
     */
    @Operation(summary = "新增单据", description = "创建前端草稿默认值，不生成正式业务单号")
    JsonVO<StocktakeDraftVO> addStocktakeDraft();

    /**
     * 保存盘点单。
     * @param dto 保存请求；busNo 为空表示新增保存，不为空表示编辑保存
     * @return 保存结果
     */
    @Operation(summary = "保存单据", description = "保存盘点单。busNo 为空时新增保存，busNo 不为空时编辑保存")
    JsonVO<StocktakeSaveResultVO> saveStocktake(StocktakeSaveDTO dto);

    /**
     * 保存并提交审批。
     * @param dto 保存请求；保存成功后进入审批流程
     * @return 保存并提交结果
     */
    @Operation(summary = "保存并提交审批", description = "先保存盘点单，再提交审批，返回审批中状态")
    JsonVO<StocktakeSubmitResultVO> saveAndSubmitStocktake(StocktakeSaveDTO dto);
}
