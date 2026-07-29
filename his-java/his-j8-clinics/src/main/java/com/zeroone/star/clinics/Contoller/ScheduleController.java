package com.zeroone.star.clinics.contoller;

import com.zeroone.star.clinics.entity.Schedule;
import com.zeroone.star.clinics.service.ScheduleService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleDTO;
import com.zeroone.star.project.j8.clinics.ClinicsApis;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@Tag(name = "门诊日程")
public class ScheduleController implements ClinicsApis {

    @Autowired
    private ScheduleService scheduleService;

    @Operation(summary = "查询日程列表")
    @Override
    @GetMapping("/query-schedule")
    public JsonVO<PageDTO<ScheduleDTO>> querySchedule(ScheduleListQuery scheduleListQuery) {
        JsonVO<PageDTO<ScheduleDTO>> pageResult= scheduleService.listQuery(scheduleListQuery);
        return null;
    }

    @Operation(summary = "获取日程详情")
    @Override
    @GetMapping("/query-schedule-detail/{scheduleId}")
    public JsonVO<ScheduleDTO> queryScheduleDetail(@PathVariable Long scheduleId) {
        ScheduleDTO schedule = scheduleService.getById(scheduleId);
        return JsonVO.success(schedule);
    }

    @Operation(summary = "新建日程")
    @Override
    @PostMapping("/add-schedule")
    public JsonVO<String> createSchedule(@RequestBody ScheduleAddDTO scheduleAddDTO) {
        scheduleService.insertSchedule(scheduleAddDTO);
        return JsonVO.success("新建成功");
    }

    @Operation(summary = "生成正式排班")
    @Override
    @PostMapping("/generate-formal-roster")
    public JsonVO<String> generateFormalRoster(@RequestBody FormalRosterDTO formalRosterDTO) {
        return null;
    }
}
