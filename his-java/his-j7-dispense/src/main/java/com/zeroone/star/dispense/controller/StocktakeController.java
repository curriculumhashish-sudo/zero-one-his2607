package com.zeroone.star.dispense.controller;

import com.zeroone.star.project.dto.j7.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.j7.stocktake.StocktakeApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.stocktake.StocktakeDraftVO;
import com.zeroone.star.project.vo.j7.stocktake.StocktakeSaveResultVO;
import com.zeroone.star.project.vo.j7.stocktake.StocktakeSubmitResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：盘点单新增保存接口控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@RestController
@RequestMapping("/api/j7/stocktake")
@Tag(name = "盘点单新增保存", description = "新增单据、保存单据、保存并提交审批接口")
public class StocktakeController implements StocktakeApis {

    @Override
    @Operation(summary = "新增单据", description = "创建前端草稿默认值，不生成正式业务单号")
    @PostMapping("/draft")
    public JsonVO<StocktakeDraftVO> addStocktakeDraft() {
        StocktakeDraftVO vo = new StocktakeDraftVO();
        vo.setDraftId("draft-" + System.currentTimeMillis());
        vo.setStatusEnum("DRAFT");
        vo.setStatusName("编辑中");
        vo.setCategoryEnum("STOCK_SUPPLY");
        vo.setDraftTime(LocalDateTime.now());
        return JsonVO.success(vo);
    }

    @Override
    @Operation(summary = "保存单据", description = "保存盘点单。busNo 为空时新增保存，busNo 不为空时编辑保存")
    @PostMapping("/save")
    public JsonVO<StocktakeSaveResultVO> saveStocktake(@Valid @RequestBody StocktakeSaveDTO dto) {
        boolean createMode = dto.getBusNo() == null || dto.getBusNo().isBlank();
        StocktakeSaveResultVO vo = new StocktakeSaveResultVO();
        vo.setBusNo(createMode ? "PD202607230001" : dto.getBusNo());
        vo.setSaveMode(createMode ? "CREATE" : "UPDATE");
        vo.setStatusEnum("PENDING_APPROVAL");
        vo.setStatusName("待审批");
        vo.setSuccess(Boolean.TRUE);
        return JsonVO.success(vo);
    }

    @Override
    @Operation(summary = "保存并提交审批", description = "先保存盘点单，再提交审批，返回审批中状态")
    @PostMapping("/save-and-submit")
    public JsonVO<StocktakeSubmitResultVO> saveAndSubmitStocktake(@Valid @RequestBody StocktakeSaveDTO dto) {
        boolean createMode = dto.getBusNo() == null || dto.getBusNo().isBlank();
        StocktakeSubmitResultVO vo = new StocktakeSubmitResultVO();
        vo.setBusNo(createMode ? "PD202607230001" : dto.getBusNo());
        vo.setSaveMode(createMode ? "CREATE" : "UPDATE");
        vo.setStatusEnum("APPROVAL");
        vo.setStatusName("审批中");
        vo.setSuccess(Boolean.TRUE);
        return JsonVO.success(vo);
    }
}
