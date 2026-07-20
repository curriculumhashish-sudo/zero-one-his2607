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

import java.util.ArrayList;
import java.util.List;

@RestController
@RequestMapping("ons")
@Tag(name = "Outpatient Nurse Station", description = "门诊护士站接口")
public class OnsController implements OnsApis {

    @Operation(summary = "执行（支持批量）", description = "批量执行医嘱，支持一次执行多条记录")
    @PostMapping("execute")
    @Override
    public JsonVO<Boolean> execute(@RequestBody ExecuteRequest request) {
        return JsonVO.success(true);
    }

    @Operation(summary = "取消执行", description = "取消已执行的记录")
    @PostMapping("cancel-execute")
    @Override
    public JsonVO<Boolean> cancelExecute(@RequestBody CancelExecuteRequest request) {
        return JsonVO.success(true);
    }

    @Operation(summary = "打印", description = "打印执行记录")
    @PostMapping("print")
    @Override
    public JsonVO<String> print(@RequestBody PrintRequest request) {
        return JsonVO.success("打印成功");
    }

    @Operation(summary = "获取执行记录（条件+分页）", description = "根据条件分页查询执行记录")
    @GetMapping("execution-records")
    @Override
    public JsonVO<PageDTO<ExecutionRecordVO>> getExecutionRecords(ExecutionRecordQuery query) {
        List<ExecutionRecordVO> records = new ArrayList<>();
        ExecutionRecordVO record = new ExecutionRecordVO();
        record.setId("1");
        record.setPatient_id("1");
        record.setPatient_name("张三");
        record.setPatient_gender("男");
        record.setPatient_age(30);
        record.setOrder_id("1");
        record.setOrder_content("阿莫西林胶囊 0.5g 口服 每日三次");
        record.setStatus(1);
        record.setStatus_name("已执行");
        record.setExecutor_id("1");
        record.setExecutor_name("李护士");
        record.setExecute_time("2024-01-15 10:30:00");
        record.setRemark("执行完成");
        records.add(record);
        PageDTO<ExecutionRecordVO> pageDTO = new PageDTO<>();
        pageDTO.setTotal(1L);
        pageDTO.setPageIndex(query.getPageIndex());
        pageDTO.setPageSize(query.getPageSize());
        pageDTO.setRows(records);
        return JsonVO.success(pageDTO);
    }

    @Operation(summary = "获取患者队列（条件+分页）", description = "根据条件分页查询患者队列")
    @GetMapping("patient-queue")
    @Override
    public JsonVO<PageDTO<PatientQueueVO>> getPatientQueue(PatientQueueQuery query) {
        List<PatientQueueVO> queues = new ArrayList<>();
        PatientQueueVO queue = new PatientQueueVO();
        queue.setId("1");
        queue.setPatient_id("1");
        queue.setPatient_name("张三");
        queue.setPatient_gender("男");
        queue.setPatient_age(30);
        queue.setPatient_card_no("P000001");
        queue.setDepartment_id("1");
        queue.setDepartment_name("内科");
        queue.setDoctor_id("1");
        queue.setDoctor_name("王医生");
        queue.setVisit_no("V20240115001");
        queue.setVisit_order(1);
        queue.setStatus(0);
        queue.setStatus_name("待就诊");
        queue.setRegister_time("2024-01-15 08:00:00");
        queue.setExpected_visit_time("2024-01-15 09:00:00");
        queues.add(queue);
        PageDTO<PatientQueueVO> pageDTO = new PageDTO<>();
        pageDTO.setTotal(1L);
        pageDTO.setPageIndex(query.getPageIndex());
        pageDTO.setPageSize(query.getPageSize());
        pageDTO.setRows(queues);
        return JsonVO.success(pageDTO);
    }

    @Operation(summary = "获取患者执行项目", description = "根据患者ID获取执行项目列表")
    @GetMapping("patient/{patientId}/execution-items")
    @Override
    public JsonVO<List<PatientExecutionItemVO>> getPatientExecutionItems(
            @Parameter(description = "患者ID", required = true, example = "1")
            @PathVariable String patientId) {
        List<PatientExecutionItemVO> items = new ArrayList<>();
        PatientExecutionItemVO item = new PatientExecutionItemVO();
        item.setId("1");
        item.setOrder_id("1");
        item.setItem_name("阿莫西林胶囊");
        item.setItem_code("MED001");
        item.setItem_type("MEDICATION");
        item.setDosage("0.5");
        item.setDosage_unit("g");
        item.setFrequency("每日三次");
        item.setStatus(0);
        item.setStatus_name("未执行");
        items.add(item);
        return JsonVO.success(items);
    }

    @Operation(summary = "获取患者耗材项目", description = "根据患者ID获取耗材项目列表")
    @GetMapping("patient/{patientId}/consumable-items")
    @Override
    public JsonVO<List<PatientConsumableItemVO>> getPatientConsumableItems(
            @Parameter(description = "患者ID", required = true, example = "1")
            @PathVariable String patientId) {
        List<PatientConsumableItemVO> items = new ArrayList<>();
        PatientConsumableItemVO item = new PatientConsumableItemVO();
        item.setId("1");
        item.setOrder_id("1");
        item.setConsumable_name("一次性注射器");
        item.setConsumable_code("CON001");
        item.setSpecification("5ml");
        item.setUnit("支");
        item.setQuantity(2);
        item.setUnit_price(2.5);
        item.setAmount(5.0);
        item.setStatus(0);
        item.setStatus_name("未使用");
        items.add(item);
        return JsonVO.success(items);
    }
}