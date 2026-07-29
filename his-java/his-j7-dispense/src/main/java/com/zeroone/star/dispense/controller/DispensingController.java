package com.zeroone.star.dispense.controller;

import com.zeroone.star.dispense.service.IDispensingService;
import com.zeroone.star.project.j7.dispense.DispensingApis;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.query.j7.dispense.DispensingSummaryQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：住院发药管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zg
 * @version 1.0.0
 */
@RestController
@Tag(name = "住院发药", description = "住院发药基础流程")
public class DispensingController implements DispensingApis {

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
}