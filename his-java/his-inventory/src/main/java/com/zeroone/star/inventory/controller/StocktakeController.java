package com.zeroone.star.inventory.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSubmitDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.stocktake.StocktakeApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeItemVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：盘点单查询提交接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@RequestMapping("/stocktake")
@Tag(name = "盘点单管理", description = "盘点单查询提交接口")
public class StocktakeController implements StocktakeApis {

    @Operation(summary = "获取盘点单列表")
    @GetMapping("/list")
    @Override
    public JsonVO<PageDTO<StocktakeListVO>> queryStocktakeList(StocktakeQuery query) {
        PageDTO<StocktakeListVO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(query.getPageIndex());
        pageDTO.setPageSize(query.getPageSize());
        pageDTO.setTotal(3L);
        pageDTO.setPages(1L);

        List<StocktakeListVO> rows = new ArrayList<>();

        StocktakeListVO vo1 = new StocktakeListVO();
        vo1.setId("1");
        vo1.setBusNo("PD20260701001");
        vo1.setStatusEnum("DRAFT");
        vo1.setWarehouseName("门诊药房");
        vo1.setProfitLossPrice(new BigDecimal("-125.50"));
        vo1.setApplicantName("张三");
        vo1.setApplyTime(LocalDateTime.of(2026, 7, 1, 10, 30, 0));
        rows.add(vo1);

        StocktakeListVO vo2 = new StocktakeListVO();
        vo2.setId("2");
        vo2.setBusNo("PD20260701002");
        vo2.setStatusEnum("SUBMITTED");
        vo2.setWarehouseName("住院药房");
        vo2.setProfitLossPrice(new BigDecimal("250.00"));
        vo2.setApplicantName("李四");
        vo2.setApplyTime(LocalDateTime.of(2026, 7, 2, 9, 0, 0));
        vo2.setApproverName("王主任");
        rows.add(vo2);

        StocktakeListVO vo3 = new StocktakeListVO();
        vo3.setId("3");
        vo3.setBusNo("PD20260701003");
        vo3.setStatusEnum("APPROVED");
        vo3.setWarehouseName("药库");
        vo3.setProfitLossPrice(new BigDecimal("-500.00"));
        vo3.setApplicantName("王五");
        vo3.setApplyTime(LocalDateTime.of(2026, 7, 3, 14, 0, 0));
        vo3.setApproverName("赵院长");
        vo3.setApprovalTime(LocalDateTime.of(2026, 7, 3, 16, 0, 0));
        rows.add(vo3);

        pageDTO.setRows(rows);
        return JsonVO.success(pageDTO);
    }

    @Operation(summary = "获取盘点单详情")
    @GetMapping("/detail/{id}")
    @Override
    public JsonVO<StocktakeDetailVO> queryStocktakeDetail(@PathVariable String id) {
        StocktakeDetailVO detailVO = new StocktakeDetailVO();
        detailVO.setId(id);
        detailVO.setBusNo("PD20260701001");
        detailVO.setStatusEnum("DRAFT");
        detailVO.setStatusDesc("草稿");
        detailVO.setWarehouseId("1");
        detailVO.setWarehouseName("门诊药房");
        detailVO.setStocktakeType("PHARMACY");
        detailVO.setTotalProfitPrice(new BigDecimal("500.00"));
        detailVO.setTotalLossPrice(new BigDecimal("625.50"));
        detailVO.setProfitLossPrice(new BigDecimal("-125.50"));
        detailVO.setItemCount(2);
        detailVO.setApplicantId("1");
        detailVO.setApplicantName("张三");
        detailVO.setApplyTime(LocalDateTime.of(2026, 7, 1, 10, 30, 0));
        detailVO.setRemark("月度盘点");
        detailVO.setCreatedAt(LocalDateTime.of(2026, 7, 1, 9, 0, 0));

        List<StocktakeItemVO> items = new ArrayList<>();

        StocktakeItemVO item1 = new StocktakeItemVO();
        item1.setId("101");
        item1.setItemId("1001");
        item1.setItemName("阿莫西林胶囊");
        item1.setSpecification("0.25g*24粒");
        item1.setLotNumber("20260101");
        item1.setBookQuantity(new BigDecimal("100"));
        item1.setTotalQuantity(new BigDecimal("95"));
        item1.setItemQuantity(new BigDecimal("-5"));
        item1.setUnitCode("box");
        item1.setUnitName("盒");
        item1.setPrice(new BigDecimal("12.50"));
        item1.setProfitLossPrice(new BigDecimal("-62.50"));
        items.add(item1);

        StocktakeItemVO item2 = new StocktakeItemVO();
        item2.setId("102");
        item2.setItemId("1002");
        item2.setItemName("布洛芬缓释胶囊");
        item2.setSpecification("0.3g*20粒");
        item2.setLotNumber("20260201");
        item2.setBookQuantity(new BigDecimal("50"));
        item2.setTotalQuantity(new BigDecimal("55"));
        item2.setItemQuantity(new BigDecimal("5"));
        item2.setUnitCode("box");
        item2.setUnitName("盒");
        item2.setPrice(new BigDecimal("25.00"));
        item2.setProfitLossPrice(new BigDecimal("125.00"));
        items.add(item2);

        detailVO.setItems(items);
        return JsonVO.success(detailVO);
    }

    @Operation(summary = "提交盘点单审批")
    @PostMapping("/submit")
    @Override
    public JsonVO<String> submitStocktake(@RequestBody @Valid StocktakeSubmitDTO dto) {
        return JsonVO.success("提交成功");
    }
}
