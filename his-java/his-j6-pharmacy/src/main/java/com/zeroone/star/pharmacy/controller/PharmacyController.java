package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StockOutDeleteDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StockOutSaveDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.query.j6.pharmacy.StockOutDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutListVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 药库管理 - 出库单与审批流转 前端控制器（桩实现，用于生成Knife4j接口文档）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j6小组-告别
 * @since 2026-07-18
 */
@RestController
@Tag(name = "pharmacy", description = "药库出库管理")
@Validated
public class PharmacyController implements PharmacyApis {

    // ==================== 出库单 CRUD ====================

    @PostMapping("/pharmacy/stock-out/save")
    @Operation(summary = "新增/编辑出库单")
    @Override
    public JsonVO<String> saveStockOut(@Validated @RequestBody StockOutSaveDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("SO202607180001");
    }

    @DeleteMapping("/pharmacy/stock-out/delete")
    @Operation(summary = "删除出库单")
    @Override
    public JsonVO<String> deleteStockOut(@Validated @RequestBody StockOutDeleteDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("ok");
    }

    @GetMapping("/pharmacy/stock-out/list")
    @Operation(summary = "分页查询出库单列表")
    @Override
    public JsonVO<PageDTO<StockOutListVO>> queryStockOuts(@Validated StockOutQuery condition) {
        // TODO: 桩实现，待后续对接Service层
        StockOutListVO row = new StockOutListVO();
        row.setBusNo("SO202607180001");
        row.setTypeEnum("CABINET_STOCK_OUT");
        row.setStatusEnum("APPROVAL");
        row.setSourceLocationName("中心药库");
        row.setPurposeLocationName("门诊药房");
        row.setSupplierName("国药控股");
        row.setItemName("阿莫西林胶囊");
        row.setItemQuantity(new BigDecimal("100.00"));
        row.setUnitCode("BOX");
        row.setPrice(new BigDecimal("15.50"));
        row.setTotalPrice(new BigDecimal("1550.00"));
        row.setApplicantName("张三");
        row.setApplyTime(LocalDateTime.now().minusHours(2));
        row.setApproverName("李四");
        row.setApprovalTime(LocalDateTime.now());

        PageDTO<StockOutListVO> page = new PageDTO<>();
        page.setPageIndex(1L);
        page.setPageSize(10L);
        page.setTotal(1L);
        page.setPages(1L);
        page.setRows(List.of(row));
        return JsonVO.success(page);
    }

    @GetMapping("/pharmacy/stock-out/detail")
    @Operation(summary = "查询出库单详情（含明细行）")
    @Override
    public JsonVO<StockOutDetailVO> queryStockOutDetail(@Validated StockOutDetailQuery query) {
        // TODO: 桩实现，待后续对接Service层
        StockOutDetailVO.StockOutDetailLineVO line = new StockOutDetailVO.StockOutDetailLineVO();
        line.setId("310000000000000701");
        line.setItemTable("med_medication_definition");
        line.setItemId("310000000000000401");
        line.setItemQuantity(new BigDecimal("100.00"));
        line.setUnitCode("BOX");
        line.setUnitCodeDictText("盒");
        line.setProductName("阿莫西林胶囊");
        line.setSpecification("0.25g*24粒");
        line.setManufacturer("华北制药");
        line.setLotNumber("B202607001");
        line.setPrice(new BigDecimal("15.50"));
        line.setTotalPrice(new BigDecimal("1550.00"));
        line.setOccurrenceTime(LocalDateTime.now().minusHours(2));
        line.setReason("科室申领");
        line.setRemake("加急出库");
        line.setStartTime(LocalDateTime.of(2026, 1, 1, 0, 0));
        line.setEndTime(LocalDateTime.of(2028, 1, 1, 0, 0));

        StockOutDetailVO detail = new StockOutDetailVO();
        detail.setBusNo("SO202607180001");
        detail.setTypeEnum("CABINET_STOCK_OUT");
        detail.setStatusEnum("APPROVAL");
        detail.setSourceLocationName("中心药库");
        detail.setPurposeLocationName("门诊药房");
        detail.setSourceTypeEnum("WAREHOUSE");
        detail.setPurposeTypeEnum("PHARMACY");
        detail.setCategoryEnum("STOCK_OUT");
        detail.setRowVersion(0);
        detail.setApplicantName("张三");
        detail.setApplyTime(LocalDateTime.now().minusHours(2));
        detail.setApproverName("李四");
        detail.setApprovalTime(LocalDateTime.now());
        detail.setLines(List.of(line));
        return JsonVO.success(detail);
    }

    // ==================== 出库审批流转 ====================

    @PostMapping("/pharmacy/stock-out/approval")
    @Operation(summary = "出库单审批操作（提交/撤回/同意/驳回四合一）")
    @Override
    public JsonVO<StockOutDetailVO> approveStockOut(@Validated @RequestBody StockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        StockOutDetailVO result = new StockOutDetailVO();
        result.setBusNo(dto.getBusNo());
        switch (dto.getAction()) {
            case "SUBMIT" -> result.setStatusEnum("APPROVAL");
            case "WITHDRAW" -> result.setStatusEnum("WITHDRAW");
            case "AGREE" -> {
                result.setStatusEnum("AGREE");
                result.setApproverName("李四");
                result.setApprovalTime(LocalDateTime.now());
            }
            case "REJECT" -> {
                result.setStatusEnum("REJECT");
                result.setReason(dto.getRejectReason() != null ? dto.getRejectReason() : "驳回原因");
                result.setApproverName("李四");
                result.setApprovalTime(LocalDateTime.now());
            }
        }
        return JsonVO.success(result);
    }
}
