package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.mapper.MsDiagnosisMapper;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisStatusDTO;
import com.zeroone.star.project.dto.j4.diagnosis.MedicalInsuranceMatchDTO;
import com.zeroone.star.project.j4.basedata.DiagnosisApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.MedicalInsuranceMatchVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：诊断管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/diagnosis")
@Tag(name = "basedata-diagnosis", description = "诊断管理接口")
public class DiagnosisController implements DiagnosisApis {

    @Resource
    private MsDiagnosisMapper msDiagnosisMapper;

    @Override
    @Operation(summary = "批量启用诊断")
    @PatchMapping("/modify-enable-status")
    public JsonVO<Void> modifyDiagnosisEnable(@Valid @RequestBody DiagnosisStatusDTO dto) {
        return JsonVO.success(null);
    }

    @Override
    @Operation(summary = "批量停用诊断")
    @PatchMapping("/modify-disable-status")
    public JsonVO<Void> modifyDiagnosisDisable(@Valid @RequestBody DiagnosisStatusDTO dto) {
        return JsonVO.success(null);
    }

    @Override
    @Operation(summary = "更新诊断医保对照")
    @PutMapping("/modify-medical-insurance")
    public JsonVO<MedicalInsuranceMatchVO> updateMedicalInsurance(
            @Valid @RequestBody MedicalInsuranceMatchDTO dto) {
        MedicalInsuranceMatchVO vo = msDiagnosisMapper.toMedicalInsuranceMatchVO(dto);
        return JsonVO.success(vo);
    }
}
