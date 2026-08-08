package com.zeroone.star.dispense.controller.end;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.dispense.service.DrugService;
import com.zeroone.star.dispense.service.PatientService;
import com.zeroone.star.project.j7.outpharm.OutPharmApis;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
import com.zeroone.star.project.dto.j7.dispense.BatchDispenseCommand;
import com.zeroone.star.project.vo.j7.dispense.BatchDispenseResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api/j7/outpharm")
@Validated
@Tag(name = "门诊发药", description = "门诊发药基础流程")
public class menzhenfayao implements OutPharmApis {

    @Resource
    private PatientService patientService;

    @Resource
    private DrugService drugService;

    @Operation(
            summary = "分页查询患者队列",
            description = "根据姓名、状态、机构等条件分页查询候诊/发药患者队列"
    )
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = JsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"records\":[],\"total\":0,\"size\":10,\"current\":1,\"pages\":0}}")))
    })
    @PostMapping("/patient-queue")
    @Override
    public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(@RequestBody PatientQueueQuery query) {
        IPage<PatientQueueVO> page = patientService.pagePatientQueue(query);
        return JsonVO.success(page);
    }

    @Operation(
            summary = "分页查询患者药品项目",
            description = "根据患者ID、就诊ID、处方状态等条件查询患者处方药品明细"
    )
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = JsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"records\":[],\"total\":0,\"size\":10,\"current\":1,\"pages\":0}}")))
    })
    @PostMapping("/patient-drug-items")
    @Override
    public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(@RequestBody PatientDrugItemQuery query) {
        IPage<PatientDrugItemVO> page = drugService.pagePatientDrugItems(query);
        return JsonVO.success(page);
    }

    @Operation(
            summary = "批量发药",
            description = "根据发药记录ID列表批量确认发药，将 PENDING 状态的门诊发药记录更新为 COMPLETED"
    )
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = JsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"successCount\":2,\"failCount\":0,\"successIds\":[\"d001\",\"d002\"],\"failIds\":[],\"message\":\"批量发药完成\"}}")))
    })
    @PostMapping("/batch-dispense")
    @Override
    public JsonVO<BatchDispenseResultVO> batchDispense(
            @RequestBody @Valid BatchDispenseCommand command) {
        BatchDispenseResultVO result = drugService.batchDispense(command);
        return JsonVO.success(result);
    }
}
