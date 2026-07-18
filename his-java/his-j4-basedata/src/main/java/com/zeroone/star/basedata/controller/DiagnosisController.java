package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.j4.diagnosis.CreateDiagnosisDTO;
import com.zeroone.star.project.dto.j4.diagnosis.UpdateDiagnosisDTO;
import com.zeroone.star.project.dto.j4.diagnosis.ToggleStatusDTO;
import com.zeroone.star.project.j4.basedata.DiagnosisApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisVO;
import com.zeroone.star.basedata.service.IDiagnosisService;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/basedata/diagnosis")
public class DiagnosisController implements DiagnosisApis {

    private final IDiagnosisService diagnosisService;

    public DiagnosisController(IDiagnosisService diagnosisService) {
        this.diagnosisService = diagnosisService;
    }

    @PostMapping
    public JsonVO<DiagnosisVO> createDiagnosis(@Valid @RequestBody CreateDiagnosisDTO request) {
        DiagnosisVO response = diagnosisService.createDiagnosis(request);
        return JsonVO.success(response);
    }

    @PutMapping("/{id}")
    public JsonVO<DiagnosisVO> updateDiagnosis(
            @PathVariable String id,
            @Valid @RequestBody UpdateDiagnosisDTO request) {
        DiagnosisVO response = diagnosisService.updateDiagnosis(id, request);
        return JsonVO.success(response);
    }

    @DeleteMapping("/{id}")
    public JsonVO<Void> deleteDiagnosis(@PathVariable String id) {
        diagnosisService.deleteDiagnosis(id);
        return JsonVO.success(null);
    }

    @PatchMapping("/{id}/status")
    public JsonVO<DiagnosisVO> toggleDiagnosisStatus(
            @PathVariable String id,
            @Valid @RequestBody ToggleStatusDTO request) {
        DiagnosisVO response = diagnosisService.toggleDiagnosisStatus(id, request.getValiFlag());
        return JsonVO.success(response);
    }
}