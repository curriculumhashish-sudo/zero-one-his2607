package com.zeroone.star.clinics.controller;

import com.zeroone.star.clinics.service.ClinicScheduleService;
import com.zeroone.star.project.dto.j8.clinics.AllDayStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.SubstituteDTO;
import com.zeroone.star.project.j8.clinics.ClinicScheduleApis;
import com.zeroone.star.project.vo.JsonVO;
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
    public JsonVO<String> setSubstitute(@Valid @RequestBody SubstituteDTO dto) {
        clinicScheduleService.setSubstitute(dto);
        return JsonVO.success("替诊设置成功");
    }

    @Operation(summary = "撤销替诊")
    @PostMapping("/cancel-substitute")
    @Override
    public JsonVO<String> cancelSubstitute(@RequestParam Long scheduleId) {
        clinicScheduleService.cancelSubstitute(scheduleId);
        return JsonVO.success("替诊撤销成功");
    }

    @Operation(summary = "整日停诊")
    @PostMapping("/execute-stop-all-day")
    @Override
    public JsonVO<String> executeAllDayStopClinic(@Valid @RequestBody AllDayStopClinicExecuteDTO dto) {
        clinicScheduleService.executeAllDayStopClinic(dto);
        return JsonVO.success("整日停诊执行成功");
    }

    @Operation(summary = "部分停诊")
    @PostMapping("/execute-stop-partial")
    @Override
    public JsonVO<String> executePartialStopClinic(@Valid @RequestBody PartialStopClinicExecuteDTO dto) {
        clinicScheduleService.executePartialStopClinic(dto);
        return JsonVO.success("部分停诊执行成功");
    }

    @Operation(summary = "解锁停诊")
    @PostMapping("/unlock-stop-clinic")
    @Override
    public JsonVO<String> unlockStopClinic(@RequestParam Long scheduleId) {
        clinicScheduleService.unlockStopClinic(scheduleId);
        return JsonVO.success("停诊解锁成功");
    }
}