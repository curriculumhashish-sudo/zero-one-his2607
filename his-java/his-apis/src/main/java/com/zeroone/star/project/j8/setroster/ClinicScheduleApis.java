package com.zeroone.star.project.j8.setroster;


import com.zeroone.star.project.dto.j8.setroster.AllDayStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.setroster.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.setroster.SubstituteDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

@Tag(name = "门诊排班管理")
public interface ClinicScheduleApis {

    @Operation(summary = "设置替诊")
    @PostMapping("/setroster/set-substitute")
    JsonVO<String> setSubstitute(@RequestBody SubstituteDTO dto);

    @Operation(summary = "撤销替诊")
    @PostMapping("/setroster/cancel-substitute")
    JsonVO<String> cancelSubstitute(@RequestParam Long scheduleId);

//    @Operation(summary = "停诊预览")
//    @PostMapping("/setroster/preview-stop-clinic")
//    JsonVO<StopClinicPreviewDTO> previewStopClinic(@RequestBody StopClinicPreviewQuery query);

    @Operation(summary = "整日停诊")
    @PostMapping("/setroster/execute-stop-all-day")
    JsonVO<String> executeAllDayStopClinic(@RequestBody AllDayStopClinicExecuteDTO dto);

    @Operation(summary = "部分停诊")
    @PostMapping("/setroster/execute-stop-partial")
    JsonVO<String> executePartialStopClinic(@RequestBody PartialStopClinicExecuteDTO dto);

    @Operation(summary = "解锁停诊")
    @PostMapping("/setroster/unlock-stop-clinic")
    JsonVO<String> unlockStopClinic(@RequestParam Long scheduleId);

//    @Operation(summary = "查询排班日程列表")
//    @GetMapping("/setroster/query-schedule-list")
//    JsonVO<PageDTO<ScheduleVO>> queryScheduleList(ScheduleQuery query);
//
//    @Operation(summary = "查询排班日程详情")
//    @GetMapping("/setroster/query-schedule")
//    JsonVO<ScheduleVO> querySchedule(@RequestParam Long id);
}