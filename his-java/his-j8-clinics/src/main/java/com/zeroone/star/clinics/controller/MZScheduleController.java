package com.zeroone.star.clinics.controller;



import com.zeroone.star.clinics.service.IWkfSlotService;
import com.zeroone.star.project.dto.j8.clinics.BatchDeleteSlotDTO;
import com.zeroone.star.project.dto.j8.clinics.RetireScheduleDTO;
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
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RestController
@Tag(name = "门诊排班管理")
@RequestMapping("/j8/clinics")
@Validated
@RequiredArgsConstructor
public class MZScheduleController implements ScheduleApis {

    private final IWkfSlotService wkfSlotService;


    @Operation(summary = "获取已放号源列表")
    @PostMapping("/get-rel-number-source")
    @Override
    public JsonVO<Map<String, Object>> getRelNumberSource(@RequestBody @Valid RelSlotQuery query) {
        List<SlotVO> slotVOList = wkfSlotService.getRelNumberSource(query);
        Map<String,Object> dataMap = new HashMap<>();
        dataMap.put("content", slotVOList);
        dataMap.put("totalCount", slotVOList.size());
        dataMap.put("pageNo", 1);
        dataMap.put("pageSize", 500);
        return JsonVO.success(dataMap);

    }

    @Operation(summary = "删除号源")
    @DeleteMapping("/delete-number-source")
    @Override
    public JsonVO<String> deleteNumberSource(@RequestBody @Valid BatchDeleteSlotDTO slotIds) {
        String slotId = slotIds.getSlotIds().get(0);
        wkfSlotService.deleteNumberSource(slotId);
        return JsonVO.success(null);
    }

    @Operation(summary = "批量放号")
    @PostMapping("/batch-release-numberSource")
    @Override
    public JsonVO<SlotBatchReleaseVO> batchReleaseNumberSource(@RequestBody @Valid SlotBatchReleaseDTO numberSourceBatchReleaseDTO) {
        SlotBatchReleaseVO result = wkfSlotService.batchReleaseNumberSource(numberSourceBatchReleaseDTO);
        return JsonVO.success(result);
    }

    @Operation(summary = "恢复开诊")
    @PostMapping("/reactivate-schedule/{scheduleId}")
    @Override
    public JsonVO<ResumeScheduleVO> reactivateSchedule(@PathVariable String scheduleId) {
        ResumeScheduleVO result = wkfSlotService.reactivateSchedule(scheduleId);
        return JsonVO.success(result);
    }

    @Operation(summary = "作废日程")
    @DeleteMapping("/retire-schedule/{scheduleId}")
    @Override
    public JsonVO<String> retireSchedule(@RequestBody @Valid RetireScheduleDTO retireScheduleDTO) {
        wkfSlotService.retireSchedule(retireScheduleDTO);
        return JsonVO.success(null);
    }
}
