package com.zeroone.star.setroster.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.AddDailyScheduleDTO;
import com.zeroone.star.project.dto.j8.setroster.DailyScheduleDTO;
import com.zeroone.star.project.j8.setroster.DailyScheduleApis;
import com.zeroone.star.project.query.j8.setroster.DailyScheduleQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.setroster.DailyScheduleOptionVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;

@RestController
// @RequestMapping("/api/daily-schedule")
@RequestMapping("/j8/setroster") // 统一模块路径前缀
@Tag(name = "日排班模板接口")
public class DailyScheduleController implements DailyScheduleApis {

    @Override
    @Operation(summary = "获取日排班模板下拉选项")
    @GetMapping("j8/setroster/listDailyScheduleOptions")
    public JsonVO<List<DailyScheduleOptionVO>> listDailyScheduleOptions() {
        return JsonVO.success(new ArrayList<>());
    }

    @Override
    @Operation(summary = "分页查询日排班模板列表")
    @GetMapping("j8/setroster/pageDailySchedule")
    public JsonVO<PageDTO<DailyScheduleDTO>> pageDailySchedule(
            DailyScheduleQuery query) {
        return JsonVO.success(new PageDTO<>(1L, 10L, 0L, 0L, new ArrayList<>()));
    }

    @Override
    @Operation(summary = "新增日排班模板")
    @PostMapping("j8/setroster/addDailySchedule")
    public JsonVO<String> addDailySchedule(AddDailyScheduleDTO dto) {
        return JsonVO.success("");
    }

    @Override
    @Operation(summary = "编辑日排班模板")
    @PutMapping("j8/setroster/modifyDailySchedule")
    public JsonVO<String> modifyDailySchedule(
            DailyScheduleDTO dto) {
        return JsonVO.success("");
    }

    @Override
    @Operation(summary = "删除日排班模板")
    @DeleteMapping("j8/setroster/deleteDailySchedule/{id}")
    public JsonVO<String> deleteDailySchedule(String id) {
        return JsonVO.success("");
    }
}
