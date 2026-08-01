package com.zeroone.star.clinics.contoller;

import com.zeroone.star.clinics.service.ScheduleService;
import com.zeroone.star.project.dto.j8.clinics.FormalRosterDTO;
import com.zeroone.star.project.dto.j8.clinics.ScheduleAddDTO;
import com.zeroone.star.project.j8.clinics.ClinicsApis;
import com.zeroone.star.project.query.j8.clinics.ScheduleListQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.clinics.GenerateRosterVO;
import com.zeroone.star.project.vo.j8.clinics.ScheduleCreateVO;
import com.zeroone.star.project.vo.j8.clinics.ScheduleItemVO;
import com.zeroone.star.project.vo.j8.clinics.ScheduleListVO;
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
    public JsonVO<ScheduleListVO> querySchedule(ScheduleListQuery scheduleListQuery) {
        ScheduleListVO result = scheduleService.listQuery(scheduleListQuery);
        return JsonVO.success(result);
    }

    @Operation(summary = "获取日程详情")
    @Override
    @GetMapping("/query-schedule-detail/{scheduleId}")
    public JsonVO<ScheduleItemVO> queryScheduleDetail(@PathVariable String scheduleId) {
        ScheduleItemVO schedule = scheduleService.getById(scheduleId);
        return JsonVO.success(schedule);
    }

    @Operation(summary = "新建日程")
    @Override
    @PostMapping("/add-schedule")
    public JsonVO<ScheduleCreateVO> createSchedule(@RequestBody ScheduleAddDTO scheduleAddDTO) {
        ScheduleCreateVO result = scheduleService.insertSchedule(scheduleAddDTO);
        return JsonVO.success(result);
    }

    @Operation(summary = "生成正式排班")
    @Override
    @PostMapping("/generate-formal-roster")
    public JsonVO<GenerateRosterVO> generateFormalRoster(@RequestBody FormalRosterDTO formalRosterDTO) {
        GenerateRosterVO result = scheduleService.generateFormalRoster(formalRosterDTO);
        return JsonVO.success(result);
    }
}