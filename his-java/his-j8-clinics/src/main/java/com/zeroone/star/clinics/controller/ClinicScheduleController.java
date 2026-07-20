        package com.zeroone.star.clinics.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.StopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.setroster.StopClinicPreviewDTO;
import com.zeroone.star.project.dto.j8.setroster.SubstituteDTO;
import com.zeroone.star.project.j8.setroster.ClinicScheduleApis;
import com.zeroone.star.project.query.j8.setroster.ScheduleQuery;
import com.zeroone.star.project.query.j8.setroster.StopClinicPreviewQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.ScheduleVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.Arrays;

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

    @Operation(summary = "停诊预览")
    @PostMapping("/preview-stop-clinic")
    @Override
    public JsonVO<StopClinicPreviewDTO> previewStopClinic(@RequestBody StopClinicPreviewQuery query) {
        StopClinicPreviewDTO result = new StopClinicPreviewDTO();
        result.setScheduleId(query.getScheduleId());
        result.setStopType(query.getStopType());
        return JsonVO.success(result);
    }

    @Operation(summary = "执行停诊")
    @PostMapping("/execute-stop-clinic")
    @Override
    public JsonVO<String> executeStopClinic(@RequestBody StopClinicExecuteDTO dto) {
        return JsonVO.success("停诊执行成功");
    }

    @Operation(summary = "解锁停诊")
    @PostMapping("/unlock-stop-clinic")
    @Override
    public JsonVO<String> unlockStopClinic(@RequestParam Long scheduleId) {
        return JsonVO.success("停诊解锁成功");
    }

    @Operation(summary = "查询排班日程列表")
    @GetMapping("/query-schedule-list")
    @Override
    public JsonVO<PageDTO<ScheduleVO>> queryScheduleList(ScheduleQuery query) {
        PageDTO<ScheduleVO> pageDTO = new PageDTO<>();
        pageDTO.setTotal(2L);

        ScheduleVO vo1 = new ScheduleVO();
        vo1.setId(1L);
        vo1.setPractitionerId(100L);
        vo1.setPractitionerName("李医生");
        vo1.setStatusEnum("active");
        vo1.setScheduleDate("2026-07-20");
        vo1.setStartTime("08:00");
        vo1.setEndTime("17:00");
        vo1.setDeptId(10L);
        vo1.setDeptName("内科");

        ScheduleVO vo2 = new ScheduleVO();
        vo2.setId(2L);
        vo2.setPractitionerId(101L);
        vo2.setPractitionerName("王医生");
        vo2.setSubstitutePractitionerId(102L);
        vo2.setSubstitutePractitionerName("张医生");
        vo2.setSubstituteReason("医师请假");
        vo2.setStatusEnum("active");
        vo2.setScheduleDate("2026-07-21");
        vo2.setStartTime("09:00");
        vo2.setEndTime("12:00");
        vo2.setDeptId(11L);
        vo2.setDeptName("外科");

        pageDTO.setRows(Arrays.asList(vo1, vo2));
        return JsonVO.success(pageDTO);
    }
    @Operation(summary = "查询排班日程详情")
    @GetMapping("/query-schedule")
    @Override
    public JsonVO<ScheduleVO> querySchedule(@RequestParam Long id) {
        ScheduleVO vo = new ScheduleVO();
        vo.setId(id);
        vo.setPractitionerName("李医生");
        vo.setStatusEnum("active");
        vo.setScheduleDate("2026-07-20");
        vo.setStartTime("08:00");
        vo.setEndTime("17:00");
        vo.setDeptName("内科");
        return JsonVO.success(vo);
    }
}
