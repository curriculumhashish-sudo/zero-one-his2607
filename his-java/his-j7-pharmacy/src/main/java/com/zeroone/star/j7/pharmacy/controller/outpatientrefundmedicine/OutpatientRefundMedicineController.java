@RestController
@RequestMapping("/outpatient-refund-medicine")
@Tag(name = "门诊退药")
@Validated
public class OutpatientRefundMedicineController implements OutpatientRefundMedicineApis {

    /**
     * 获取待退药患者队列
     */
    @GetMapping("/query-patient-queue")
    @Operation(summary = "获取待退药患者队列")
    @Override
    public JsonVO<PageDTO<OutpatientRefundPatientVO>> queryPatientQueue(
            @Validated OutpatientRefundPatientQuery query) {
        OutpatientRefundPatientVO row = new OutpatientRefundPatientVO();
        row.setPatientId("326940849401761792");
        row.setEncounterId("329874660147597312");
        row.setName("李四");
        row.setGenderEnum("1");
        row.setBusNo("ZY20260719001");
        row.setOrgId("322963602326687744");
        row.setWardName("内科病区");
        row.setLocationId("322663218269917184");
        row.setPharmacyName("门诊药房");
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
    public JsonVO<List<OutpatientRefundDetailVO>> queryRefundDetail(
            @Validated OutpatientRefundDetailQuery query) {
        OutpatientRefundDetailVO detail = new OutpatientRefundDetailVO();
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
