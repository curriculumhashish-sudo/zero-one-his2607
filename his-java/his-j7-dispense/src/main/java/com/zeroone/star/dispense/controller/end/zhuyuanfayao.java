package com.zeroone.star.dispense.controller.end;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.dispense.service.IDispensingService;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.j7.DispenseApis;
import com.zeroone.star.project.query.j7.dispense.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.*;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import java.util.List;

@RestController
@Tag(name = "住院发药记录", description = "住院发药记录基础流程")
public class zhuyuanfayao implements DispenseApis {
    @Resource
    private IDispensingService dispensingService;

    @Operation(summary = "获取汇总单队列")
    @PostMapping("query-summary-list")
    @Override
    public JsonVO<ExtendPageDTO<DispensingSummaryVO>> querySummaryList(@RequestBody DispensingSummaryQuery query) {
        ExtendPageDTO<DispensingSummaryVO> page = dispensingService.querySummaryList(query);
        return JsonVO.success(page);
    }

    @Operation(summary = "获取患者药品明细")
    @GetMapping("query-patient-medications")
    @Override
    public JsonVO<List<PatientMedicationVO>> queryPatientMedications(@Parameter(description = "汇总单号", example = "1")@RequestParam String summaryNo) {
        List<PatientMedicationVO> list = dispensingService.queryPatientMedications(summaryNo);
        return JsonVO.success(list);
    }

    @Operation(summary = "核对发药")
    @PostMapping("verify-dispense")
    @Override
    public JsonVO<String> verifyDispense(@RequestBody DispenseVerifyDTO verifyDTO) {
        String result = dispensingService.verifyDispense(verifyDTO);
        return JsonVO.success(result);
    }

    // ========== 以下为 DispenseApis 接口中未实现方法的桩代码 ==========

    @Override public JsonVO<PageDTO<InpatientDispenseRecordVO>> queryPage(InpatientDispenseRecordQuery query) { throw new UnsupportedOperationException(); }
    @Override public void export(InpatientDispenseRecordQuery query, HttpServletResponse response) throws IOException { throw new UnsupportedOperationException(); }
    @Override public JsonVO<PageDTO<InpatientRefundPatientVO>> queryPatientQueue(InpatientRefundPatientQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<List<InpatientRefundDetailVO>> queryRefundDetail(InpatientRefundDetailQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<PageDTO<DispenseGroupVO>> queryPage(DispenseQuery query) { throw new UnsupportedOperationException(); }
    @Override public void export(DispenseQuery query, HttpServletResponse response) throws IOException { throw new UnsupportedOperationException(); }
    @Override public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(PatientQueueQuery query) { throw new UnsupportedOperationException(); }
    @Override public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(PatientDrugItemQuery query) { throw new UnsupportedOperationException(); }
}
