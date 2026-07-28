package com.zeroone.star.dispense.controller.end;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.dispense.service.IInpatientRefundMedicineService;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.j7.DispenseApis;
import com.zeroone.star.project.query.j7.dispense.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.*;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.IOException;
import java.util.List;

@RestController
@RequestMapping("/api/j7/refund-medicine")
@Tag(name = "住院退药", description = "住院退药（患者队列、退药明细）")
@Validated
public class zhuyuantuiyao implements DispenseApis {

    @Resource
    private IInpatientRefundMedicineService inpatientRefundMedicineService;

    @PostMapping("/page-patient-queue")
    @Operation(summary = "分页查询退药患者队列")
    @Override
    public JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(
            @Validated @RequestBody InpatientRefundPatientQuery query) {
        return JsonVO.success(inpatientRefundMedicineService.queryPatientQueue(query));
    }

    @PostMapping("/get-refund-detail")
    @Operation(summary = "获取患者退药明细")
    @Override
    public JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(
            @Validated @RequestBody InpatientRefundDetailQuery query) {
        return JsonVO.success(inpatientRefundMedicineService.queryRefundDetail(query));
    }

    // ========== 以下为 DispenseApis 接口中未实现方法的桩代码 ==========

    @Override public JsonVO<ExtendPageDTO<DispensingSummaryVO>> querySummaryList(DispensingSummaryQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<List<PatientMedicationVO>> queryPatientMedications(String summaryNo) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<String> verifyDispense(DispenseVerifyDTO verifyDTO) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<PageDTO<InpatientDispenseRecordVO>> queryPage(InpatientDispenseRecordQuery query) { throw new UnsupportedOperationException(); }
    @Override public void export(InpatientDispenseRecordQuery query, HttpServletResponse response) throws IOException { throw new UnsupportedOperationException(); }
    @Override public JsonVO<PageDTO<DispenseGroupVO>> queryPage(DispenseQuery query) { throw new UnsupportedOperationException(); }
    @Override public void export(DispenseQuery query, HttpServletResponse response) throws IOException { throw new UnsupportedOperationException(); }
    @Override public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(PatientQueueQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(PatientDrugItemQuery query) { throw new UnsupportedOperationException(); }
}
