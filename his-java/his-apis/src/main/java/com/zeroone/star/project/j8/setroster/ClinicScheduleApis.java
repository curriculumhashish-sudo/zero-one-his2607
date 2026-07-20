package com.zeroone.star.project.j8.setroster;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.StopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.setroster.StopClinicPreviewDTO;
import com.zeroone.star.project.dto.j8.setroster.SubstituteDTO;
import com.zeroone.star.project.query.j8.setroster.ScheduleQuery;
import com.zeroone.star.project.query.j8.setroster.StopClinicPreviewQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.ScheduleVO;
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

    @Operation(summary = "停诊预览")
    @PostMapping("/setroster/preview-stop-clinic")
    JsonVO<StopClinicPreviewDTO> previewStopClinic(@RequestBody StopClinicPreviewQuery query);

    @Operation(summary = "执行停诊")
    @PostMapping("/setroster/execute-stop-clinic")
    JsonVO<String> executeStopClinic(@RequestBody StopClinicExecuteDTO dto);

    @Operation(summary = "解锁停诊")
    @PostMapping("/setroster/unlock-stop-clinic")
    JsonVO<String> unlockStopClinic(@RequestParam Long scheduleId);

    @Operation(summary = "查询排班日程列表")
    @GetMapping("/setroster/query-schedule-list")
    JsonVO<PageDTO<ScheduleVO>> queryScheduleList(ScheduleQuery query);

    @Operation(summary = "查询排班日程详情")
    @GetMapping("/setroster/query-schedule")
    JsonVO<ScheduleVO> querySchedule(@RequestParam Long id);
}