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

    @Override
    public JsonVO<StocktakeDetailVO> createStocktake(StocktakeSaveDTO dto) {
        return JsonVO.success(buildExampleDetailVO());
    }

    @Override
    public JsonVO<StocktakeDetailVO> saveStocktake(String busNo, StocktakeSaveDTO dto) {
        StocktakeDetailVO vo = buildExampleDetailVO();
        vo.setBusNo(busNo);
        return JsonVO.success(vo);
    }

    @Override
    public JsonVO<StocktakeDetailVO> submitStocktake(String busNo, StocktakeSaveDTO dto) {
        StocktakeDetailVO vo = buildExampleDetailVO();
        vo.setBusNo(busNo);
        vo.setStatusEnum("PENDING_APPROVAL");
        return JsonVO.success(vo);
    }

    /** Knife4j 示例数据 */
    private StocktakeDetailVO buildExampleDetailVO() {
        StocktakeDetailVO vo = new StocktakeDetailVO();
        vo.setBusNo("PD202607180001");
        vo.setStatusEnum("DRAFT");
        vo.setPurposeLocationId("284503862625177600");
        vo.setPurposeLocationName("西药库");
        vo.setPractitionerId("322733572165734400");
        vo.setApplicantId("322733572165734400");
        vo.setApplyTime(LocalDateTime.of(2026, 7, 18, 14, 0, 0));
        vo.setApproverId("322733572165734401");
        vo.setApprovalTime(LocalDateTime.of(2026, 7, 18, 15, 0, 0));

        StocktakeDetailVO.StocktakeDetailLineVO line =
                new StocktakeDetailVO.StocktakeDetailLineVO();
        line.setLineId("line_001");
        line.setItemTable("medication");
        line.setItemId("med_001");
        line.setProductName("阿莫西林胶囊");
        line.setSpecification("0.25g*24粒");
        line.setManufacturer("华北制药");
        line.setLotNumber("LOT20260701");
        line.setExpirationDate("2028-06-30");
        line.setUnitCode("PCS");
        line.setBookQuantity(new BigDecimal("95.00"));
        line.setProfitLossQuantity(new BigDecimal("5.00"));
        line.setActualQuantity(new BigDecimal("100.00"));
        line.setProfitLossAmount(new BigDecimal("125.50"));
        line.setPrice(new BigDecimal("25.10"));
        line.setInventoryItemId("inv_001");
        line.setSupplierId("supplier_001");
        line.setTraceNo("TRACE001");
        line.setRemake("盘点正常");
        line.setOccurrenceTime(LocalDateTime.of(2026, 7, 18, 10, 0, 0));

        vo.setLines(List.of(line));
        return vo;
    }
}
