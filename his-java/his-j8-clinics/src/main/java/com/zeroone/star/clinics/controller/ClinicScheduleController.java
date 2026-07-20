package com.zeroone.star.clinics.controller;

import com.zeroone.star.project.dto.j8.setroster.AllDayStopClinicExecuteDTO;

import com.zeroone.star.project.dto.j8.setroster.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.setroster.SubstituteDTO;
import com.zeroone.star.project.j8.setroster.ClinicScheduleApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

@RequestMapping("/api/setroster")
@RestController
@Tag(name = "门诊排班管理")
public class ClinicScheduleController implements ClinicScheduleApis {

    @Operation(summary = "设置替诊")
    @PostMapping("/set-substitute")
    @Override
    public JsonVO<String> setSubstitute(@RequestBody SubstituteDTO dto) {
        return JsonVO.success("替诊设置成功");
    }

    @Operation(summary = "撤销替诊")
    @PostMapping("/cancel-substitute")
    @Override
    public JsonVO<String> cancelSubstitute(@RequestParam Long scheduleId) {
        return JsonVO.success("替诊撤销成功");
    }


    @Operation(summary = "整日停诊")
    @PostMapping("/execute-stop-all-day")
    @Override
    public JsonVO<String> executeAllDayStopClinic(@RequestBody AllDayStopClinicExecuteDTO dto) {
        return JsonVO.success("整日停诊执行成功");
    }

    @Operation(summary = "部分停诊")
    @PostMapping("/execute-stop-partial")
    @Override
    public JsonVO<String> executePartialStopClinic(@RequestBody PartialStopClinicExecuteDTO dto) {
        return JsonVO.success("部分停诊执行成功");
    }

    @Operation(summary = "解锁停诊")
    @PostMapping("/unlock-stop-clinic")
    @Override
    public JsonVO<String> unlockStopClinic(@RequestParam Long scheduleId) {
        return JsonVO.success("停诊解锁成功");
    }


}
