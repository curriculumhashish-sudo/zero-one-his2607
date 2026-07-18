package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.j4.diagnosis.CreateDiagnosisDTO;
import com.zeroone.star.project.dto.j4.diagnosis.UpdateDiagnosisDTO;
import com.zeroone.star.project.dto.j4.diagnosis.ToggleStatusDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisVO;
import org.springframework.web.bind.annotation.*;

@RequestMapping("/basedata/diagnosis")
public interface DiagnosisApis {

    @PostMapping
    JsonVO<DiagnosisVO> createDiagnosis(@RequestBody CreateDiagnosisDTO request);

    @PutMapping("/{id}")
    JsonVO<DiagnosisVO> updateDiagnosis(@PathVariable String id, @RequestBody UpdateDiagnosisDTO request);

    @DeleteMapping("/{id}")
    JsonVO<Void> deleteDiagnosis(@PathVariable String id);

    @PatchMapping("/{id}/status")
    JsonVO<DiagnosisVO> toggleDiagnosisStatus(@PathVariable String id, @RequestBody ToggleStatusDTO request);
}