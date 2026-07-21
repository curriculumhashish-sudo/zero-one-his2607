package com.zeroone.star.clinics.contoller;



import com.zeroone.star.project.dto.j8.clinics.SlotBatchReleaseDTO;
import com.zeroone.star.project.j8.clinics.ScheduleApis;
import com.zeroone.star.project.query.j8.clinics.RelSlotQuery;
import com.zeroone.star.project.vo.JsonVO;

import com.zeroone.star.project.vo.j8.clinics.ResumeScheduleVO;
import com.zeroone.star.project.vo.j8.clinics.SlotBatchReleaseVO;
import com.zeroone.star.project.vo.j8.clinics.SlotVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@Tag(name = "门诊排班管理")
@RequestMapping("/api/clinics")
@Validated
public class MZScheduleController implements ScheduleApis {

    @Operation(summary = "获取已放号源列表")
    @GetMapping("/getRelNumberSource")
    @Override
    public JsonVO<List<SlotVO>> getRelNumberSource(@Valid RelSlotQuery query) {
        return null;
    }

    @Operation(summary = "删除号源")
    @DeleteMapping("/{slotId}")
    @Override
    public JsonVO<String> deleteRelNumberSource(@PathVariable @NotBlank(message = "slotId不能为空") String slotId) {
        return null;
    }

    @Operation(summary = "批量放号")
    @PostMapping("/batchReleaseNumberSource")
    @Override
    public JsonVO<SlotBatchReleaseVO> batchReleaseNumberSource(@RequestBody @Valid SlotBatchReleaseDTO numberSourceBatchReleaseDTO) {
        return null;
    }

    @Operation(summary = "恢复开诊")
    @PostMapping("/{scheduleId}")
    @Override
    public JsonVO<ResumeScheduleVO> reactivateSchedule(@PathVariable @NotBlank(message = "scheduleId不能为空") String scheduleId) {
        return null;
    }

    @Operation(summary = "作废日程")
    @DeleteMapping("/{scheduleId}")
    @Override
    public JsonVO<String> retireSchedule(@PathVariable @NotBlank(message = "scheduleId不能为空") String scheduleId) {
        return null;
    }
}
