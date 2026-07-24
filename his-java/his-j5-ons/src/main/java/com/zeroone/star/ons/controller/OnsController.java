package com.zeroone.star.ons.controller;

import com.zeroone.star.ons.service.IOnsService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ons.*;
import com.zeroone.star.project.j5.ons.OnsApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.ons.ExecutionRecordVO;
import com.zeroone.star.project.vo.j5.ons.PatientQueueVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("ons")
@Tag(name = "Outpatient Nurse Station", description = "门诊护士站接口")
public class OnsController implements OnsApis {

    @Resource
    private IOnsService onsService;

    @Operation(summary = "执行服务请求（支持批量）", description = "批量执行医嘱/服务请求")
    @PostMapping("execute")
    @Override
    public JsonVO<Boolean> execute(@Valid @RequestBody ExecuteRequest request) {
        return onsService.execute(request);
    }

    @Operation(summary = "取消执行", description = "取消已执行的服务请求")
    @PostMapping("cancel-execute")
    @Override
    public JsonVO<Boolean> cancelExecute(@Valid @RequestBody CancelExecuteRequest request) {
        return onsService.cancelExecute(request);
    }

    @Operation(summary = "打印服务请求", description = "打印服务请求记录")
    @PostMapping("print")
    @Override
    public JsonVO<String> print(@Valid @RequestBody PrintRequest request) {
        return onsService.print(request);
    }

    @Operation(summary = "获取服务请求列表（条件+分页）", description = "根据条件分页查询服务请求")
    @GetMapping("service-requests")
    @Override
    public JsonVO<PageDTO<ExecutionRecordVO>> getServiceRequests(ExecutionRecordQuery query) {
        return onsService.getServiceRequests(query);
    }

    @Operation(summary = "获取预约队列（条件+分页）", description = "根据条件分页查询预约队列")
    @GetMapping("patient-queue")
    @Override
    public JsonVO<PageDTO<PatientQueueVO>> getPatientQueue(PatientQueueQuery query) {
        return onsService.getPatientQueue(query);
    }

    @Operation(summary = "获取患者服务请求", description = "根据患者ID获取服务请求列表")
    @GetMapping("patient/{patientId}/service-requests")
    @Override
    public JsonVO<List<ExecutionRecordVO>> getPatientServiceRequests(
            @Parameter(description = "患者ID", required = true, example = "1")
            @PathVariable String patientId) {
        return onsService.getPatientServiceRequests(patientId);
    }
}