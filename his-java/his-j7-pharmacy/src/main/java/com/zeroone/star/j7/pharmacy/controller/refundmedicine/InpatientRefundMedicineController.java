package com.zeroone.star.j7.pharmacy.controller.refundmedicine;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j7.refundmedicine.InpatientRefundMedicineApis;
import com.zeroone.star.project.query.j7.refundmedicine.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.refundmedicine.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.refundmedicine.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.refundmedicine.InpatientRefundPatientVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.Collections;
import java.util.List;

/**
 * <p>
 * 描述：住院退药查询控制器（当前返回假数据，用于 Knife4j 接口文档自测）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j7
 * @version 1.0.0
 */
@RestController
@RequestMapping("/refund-medicine")
@Tag(name = "住院退药")
@Validated
public class InpatientRefundMedicineController implements InpatientRefundMedicineApis {

    /**
     * 获取待退药患者队列
     */
    @GetMapping("/query-patient-queue")
    @Operation(summary = "获取待退药患者队列")
    @Override
    public JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(
            @Validated InpatientRefundPatientQuery query) {
        InpatientRefundPatientVO row = new InpatientRefundPatientVO();
        row.setPatientId("326940849401761792");
        row.setEncounterId("329874660147597312");
        row.setName("张三");
        row.setGenderEnum("1");
        row.setBusNo("ZY20260719001");
        row.setOrgId("322963602326687744");
        row.setWardName("内科病区");
        row.setLocationId("322663218269917184");
        row.setPharmacyName("住院药房");
        row.setRefundCount(3);
        row.setLatestApplyTime(LocalDateTime.of(2026, 7, 19, 10, 30));

        PageDTO<InpatientRefundPatientVO> page = new PageDTO<>();
        page.setPageIndex(query.getPageIndex() > 0 ? query.getPageIndex() : 1L);
        page.setPageSize(query.getPageSize() > 0 ? query.getPageSize() : 10L);
        page.setTotal(1L);
        page.setPages(1L);
        page.setRows(Collections.singletonList(row));

        return JsonVO.success(page);
    }

    /**
     * 获取患者退药明细
     */
    @GetMapping("/query-refund-detail")
    @Operation(summary = "获取患者退药明细")
    @Override
    public JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(
            @Validated InpatientRefundDetailQuery query) {
        InpatientRefundDetailVO detail = new InpatientRefundDetailVO();
        detail.setId("329906419476205568");
        detail.setRefundMedicineId("329891428408758272");
        detail.setPatientId(query.getPatientId());
        detail.setEncounterId(query.getEncounterId());
        detail.setMedicationId("322997883812974592");
        detail.setMedicationName("阿莫西林胶囊");
        detail.setMerchandiseName("0.25g*24粒");
        detail.setCategoryCode("western");
        detail.setTherapyEnum("TEMPORARY");
        detail.setQuantity(new BigDecimal("1.00"));
        detail.setUnitCode("8");
        detail.setLotNumber("20260501");
        detail.setDosageInstruction("口服，一日三次");
        detail.setMethodCode("404");
        detail.setStatusEnum("PENDING_REFUND");
        detail.setReqAuthoredTime(LocalDateTime.of(2026, 7, 19, 10, 30));
        detail.setBusNo("MR2026-07-190001");

        return JsonVO.success(Collections.singletonList(detail));
    }
}
