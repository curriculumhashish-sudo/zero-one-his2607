package com.zeroone.star.project.j5.ons;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ons.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.ons.BatchExecuteResultVO;
import com.zeroone.star.project.vo.j5.ons.ExecutionRecordVO;
import com.zeroone.star.project.vo.j5.ons.PatientConsumableItemVO;
import com.zeroone.star.project.vo.j5.ons.PatientQueueVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 门诊护士站API接口
 */
@Tag(name = "门诊护士站", description = "门诊护士站接口")
@RequestMapping("/ons")
public interface OnsApis {

    @Operation(summary = "获取患者队列（条件+分页）", description = "根据条件分页查询患者队列")
    @GetMapping("/patient-queue")
    JsonVO<PageDTO<PatientQueueVO>> getPatientQueue(PatientQueueQuery query);

    @Operation(summary = "获取患者执行项目", description = "根据患者ID获取执行项目列表")
    @GetMapping("/patient/{patientId}/service-requests")
    JsonVO<List<ExecutionRecordVO>> getPatientServiceRequests(
            @Parameter(description = "患者ID", required = true, example = "1")
            @PathVariable String patientId);

    @Operation(summary = "获取患者耗材项目", description = "根据患者ID获取耗材项目列表")
    @GetMapping("/patient/{patientId}/consumable-items")
    JsonVO<List<PatientConsumableItemVO>> getPatientConsumableItems(
            @Parameter(description = "患者ID", required = true, example = "1")
            @PathVariable String patientId);

    @Operation(summary = "执行服务请求（支持批量）", description = "批量执行医嘱/服务请求")
    @PostMapping("/execute")
    JsonVO<BatchExecuteResultVO> execute(@Valid @RequestBody ExecuteRequest request);

    @Operation(summary = "取消执行（支持批量）", description = "批量取消已执行的服务请求")
    @PostMapping("/cancel-execute")
    JsonVO<BatchExecuteResultVO> cancelExecute(@Valid @RequestBody CancelExecuteRequest request);

    @Operation(summary = "打印服务请求", description = "打印服务请求记录")
    @PostMapping("/print")
    JsonVO<String> print(@Valid @RequestBody PrintRequest request);

    @Operation(summary = "获取执行记录（条件+分页）", description = "根据条件分页查询执行记录")
    @GetMapping("/service-requests")
    JsonVO<PageDTO<ExecutionRecordVO>> getServiceRequests(ExecutionRecordQuery query);
}
