package com.zeroone.star.ons.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ons.*;
import com.zeroone.star.project.j5.ons.OnsApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.ons.*;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("ons")
@Tag(name = "Outpatient Nurse Station", description = "门诊护士站接口")
public class OnsController implements OnsApis {

    @Operation(summary = "执行（支持批量）", description = "批量执行医嘱，支持一次执行多条记录")
    @PostMapping("execute")
    @Override
    public JsonVO<Boolean> execute(@RequestBody ExecuteRequest request) {
        return null;
    }

    @Operation(summary = "取消执行", description = "取消已执行的记录")
    @PostMapping("cancel-execute")
    @Override
    public JsonVO<Boolean> cancelExecute(@RequestBody CancelExecuteRequest request) {
        return null;
    }

    @Operation(summary = "打印", description = "打印执行记录")
    @PostMapping("print")
    @Override
    public JsonVO<String> print(@RequestBody PrintRequest request) {
        return null;
    }

    @Operation(summary = "获取执行记录（条件+分页）", description = "根据条件分页查询执行记录")
    @GetMapping("execution-records")
    @Override
    public JsonVO<PageDTO<ExecutionRecordVO>> getExecutionRecords(ExecutionRecordQuery query) {
        return null;
    }

    @Operation(summary = "获取患者队列（条件+分页）", description = "根据条件分页查询患者队列")
    @GetMapping("patient-queue")
    @Override
    public JsonVO<PageDTO<PatientQueueVO>> getPatientQueue(PatientQueueQuery query) {
        return null;
    }

    @Operation(summary = "获取患者执行项目", description = "根据患者ID获取执行项目列表")
    @GetMapping("patient/{patientId}/execution-items")
    @Override
    public JsonVO<List<PatientExecutionItemVO>> getPatientExecutionItems(
            @Parameter(description = "患者ID", required = true, example = "1")
            @PathVariable String patientId) {
        return null;
    }

    @Operation(summary = "获取患者耗材项目", description = "根据患者ID获取耗材项目列表")
    @GetMapping("patient/{patientId}/consumable-items")
    @Override
    public JsonVO<List<PatientConsumableItemVO>> getPatientConsumableItems(
            @Parameter(description = "患者ID", required = true, example = "1")
            @PathVariable String patientId) {
        return null;
    }
}