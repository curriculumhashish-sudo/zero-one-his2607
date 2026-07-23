package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSaveDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.StocktakeDetailVO;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.RestController;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

@RestController
@RequiredArgsConstructor
public class PharmacyController implements PharmacyApis {

    /**
     * 新增盘点单
     * <p>生成空盘点单头，不生成明细行（明细由前端逐行添加后通过 saveStocktake 保存）</p>
     */
    @Override
    public JsonVO<StocktakeDetailVO> createStocktake(StocktakeSaveDTO dto) {
        // TODO [Dome] Step1: 生成 bus_no = "PD" + yyyyMMdd + 4位流水号(Redis INCR 或 DB 序列)

        // TODO [Dome] Step2: 在 wkf_supply_request 插入头记录:
        //   - bus_no, status_enum=DRAFT, type_enum/category_enum=盘点类型枚举
        //   - purpose_location_id=dto.purposeLocationId, practitioner_id=dto.practitionerId
        //   - item_table/unit_code 占位 ""
        //   - applicant_id=当前用户, created_at=now

        // TODO [Dome] Step3: 不生成明细行，lines 返回空列表

        // TODO [Dome] Step4: 组装 StocktakeDetailVO(头字段 + lines=[])
        return JsonVO.success(buildExampleDetailVO());
    }

    /**
     * 保存盘点单
     * <p>先删后插策略：快照锁定账面数 + 从库存项补全物品信息 + 批量写入明细行</p>
     */
    @Override
    public JsonVO<StocktakeDetailVO> saveStocktake(String busNo, StocktakeSaveDTO dto) {
        // TODO [Dome] Step1: 校验 busNo 存在且 status_enum=DRAFT，否则抛 BusinessException("盘点单不存在或状态不允许编辑")

        // TODO [Dome] Step2: 遍历 dto.lines，通过 inventoryItemId 查询 wkf_inventory_item:
        //   - 快照锁定账面数 → bookQuantity (取当前库存 item_quantity 作为账面数)
        //   - 补全展示字段: item_table, item_id, product_name, specification, manufacturer,
        //                   lot_number, expiration_date, unit_code, price, supplier_id

        // TODO [Dome] Step3: 计算 profitLossAmount = profitLossQuantity × price

        // TODO [Dome] Step4: 先删后插 — DELETE wkf_supply_request WHERE bus_no=? → INSERT 新行
        //   - 每条行写入: item_quantity=profitLossQuantity, total_quantity=actualQuantity,
        //     occurrence_time=now, status_enum=DRAFT

        // TODO [Dome] Step5: 更新头: practitioner_id, occurrence_time=now

        // TODO [Dome] Step6: 组装 StocktakeDetailVO(完整补全行, 状态=DRAFT)
        StocktakeDetailVO vo = buildExampleDetailVO();
        vo.setBusNo(busNo);
        return JsonVO.success(vo);
    }

    /**
     * 保存盘点单并提交审批
     * <p>保存盘点数据 + 状态流转 DRAFT → PENDING_APPROVAL</p>
     */
    @Override
    public JsonVO<StocktakeDetailVO> submitStocktake(String busNo, StocktakeSaveDTO dto) {
        // TODO [Dome] Step1: 同 saveStocktake Step1-5 (快照+补全+先删后插保存所有盘点数据)

        // TODO [Dome] Step2: 更新 wkf_supply_request 头:
        //   - status_enum=PENDING_APPROVAL
        //   - applicant_id=当前用户
        //   - apply_time=now

        // TODO [Dome] Step3: 组装 StocktakeDetailVO(状态=PENDING_APPROVAL)
        StocktakeDetailVO vo = buildExampleDetailVO();
        vo.setBusNo(busNo);
        vo.setStatusEnum("PENDING_APPROVAL");
        return JsonVO.success(vo);
    }

    /** Knife4j 示例数据 */
    private StocktakeDetailVO buildExampleDetailVO() {
        StocktakeDetailVO vo = new StocktakeDetailVO();
        vo.setBusNo("CST2026-07-200001");
        vo.setStatusEnum("DRAFT");
        vo.setPurposeLocationId("284503862625177600");
        vo.setPurposeLocationName("西药库");
        vo.setPractitionerId("322733572165734400");
        vo.setApplicantId("322733572165734400");
        vo.setApplyTime(LocalDateTime.of(2026, 7, 18, 14, 0, 0));
        vo.setApproverId("322733572165734401");
        vo.setCategoryEnum("WAREHOUSE_STOCKTAKE");
        vo.setApprovalTime(LocalDateTime.of(2026, 7, 18, 15, 0, 0));

        StocktakeDetailVO.StocktakeDetailLineVO line =
                new StocktakeDetailVO.StocktakeDetailLineVO();
        line.setLineId("line_001");
        line.setItemTable("medication");
        line.setItemId("med_001");
        line.setProductName("阿莫西林胶囊");
        line.setSpecification("0.25g*24粒");
        line.setManufacturer("华北制药");
        line.setLotNumber("001-004");
        line.setExpirationDate("2028-06-30");
        line.setUnitCode("PCS");
        line.setUnitName("片");
        line.setBookQuantity(new BigDecimal("95.00"));
        line.setProfitLossQuantity(new BigDecimal("5.00"));
        line.setActualQuantity(new BigDecimal("100.00"));
        line.setProfitLossAmount(new BigDecimal("125.50"));
        line.setPrice(new BigDecimal("25.10"));
        line.setInventoryItemId("inv_001");
        line.setSupplierId("supplier_001");
        line.setTraceNo("TRACE001");
        line.setRemake("盘点正常");
        line.setActualAmount(new BigDecimal("2510.00"));
        line.setOccurrenceTime(LocalDateTime.of(2026, 7, 18, 10, 0, 0));

        vo.setLines(List.of(line));
        return vo;
    }
}
