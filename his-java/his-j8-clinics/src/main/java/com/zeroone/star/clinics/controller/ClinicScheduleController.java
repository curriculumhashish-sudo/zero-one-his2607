package com.zeroone.star.clinics.controller;

import com.zeroone.star.clinics.service.ClinicScheduleService;
import com.zeroone.star.project.dto.j8.clinics.AllDayStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.SubstituteDTO;
import com.zeroone.star.project.dto.j8.clinics.UnlockStopClinicDTO;
import com.zeroone.star.project.j8.clinics.ClinicScheduleApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.clinics.StopClinicResultVO;
import com.zeroone.star.project.vo.j8.clinics.SubstituteVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

@RequestMapping("/j8/clinics")
@RestController
@Tag(name = "门诊排班管理")
@RequiredArgsConstructor
public class ClinicScheduleController implements ClinicScheduleApis {

    private final ClinicScheduleService clinicScheduleService;

    @Operation(summary = "设置替诊")
    @PostMapping("/set-substitute")
    @Override
    public JsonVO<SubstituteVO> setSubstitute(@Valid @RequestBody SubstituteDTO dto) {
        SubstituteVO vo = clinicScheduleService.setSubstitute(dto);
        return JsonVO.success(vo);
    }

    @Operation(summary = "撤销替诊")
    @PostMapping("/cancel-substitute")
    @Override
    public JsonVO<SubstituteVO> cancelSubstitute(@RequestParam String scheduleId) {
        SubstituteVO vo = clinicScheduleService.cancelSubstitute(scheduleId);
        return JsonVO.success(vo);
    }

    @Operation(summary = "整日停诊")
    @PostMapping("/execute-stop-all-day")
    @Override
    public JsonVO<StopClinicResultVO> executeAllDayStopClinic(@Valid @RequestBody AllDayStopClinicExecuteDTO dto) {
        StopClinicResultVO vo = clinicScheduleService.executeAllDayStopClinic(dto);
        return JsonVO.success(vo);
    }

    @Operation(summary = "部分停诊")
    @PostMapping("/execute-stop-partial")
    @Override
    public JsonVO<StopClinicResultVO> executePartialStopClinic(@Valid @RequestBody PartialStopClinicExecuteDTO dto) {
        StopClinicResultVO vo = clinicScheduleService.executePartialStopClinic(dto);
        return JsonVO.success(vo);
    }

    @Operation(summary = "解锁停诊")
    @PostMapping("/unlock-stop-clinic")
    @Override
    public JsonVO<String> unlockStopClinic(@Valid @RequestBody UnlockStopClinicDTO dto) {
        clinicScheduleService.unlockStopClinic(dto);
        return JsonVO.success("停诊解锁成功");
    }
}