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
import com.zeroone.star.project.vo.j7.dispense.BatchDispenseResultJsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemPageJsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueuePageJsonVO;
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
            content = @Content(schema = @Schema(implementation = PatientQueuePageJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"records\":[{\"patientId\":\"326940849401761792\",\"encounterId\":\"329874660147597312\",\"name\":\"周女士\",\"age\":26,\"genderEnum\":\"FEMALE\",\"phone\":\"13854489876\",\"statusEnum\":\"COMPLETED\",\"dispenseEnum\":\"OUTPATIENT\",\"drugCount\":6,\"createdAt\":\"2026-06-29 16:54:17\"}],\"total\":1,\"size\":10,\"current\":1,\"pages\":1}}")))
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
            content = @Content(schema = @Schema(implementation = PatientDrugItemPageJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"records\":[{\"id\":\"329907418609750016\",\"tenantId\":\"1\",\"busNo\":\"MDP2026-06-290013\",\"statusEnum\":\"COMPLETED\",\"dispenseEnum\":\"OUTPATIENT\",\"medicationId\":\"322997883812974592\",\"patientId\":\"326940849401761792\",\"encounterId\":\"329874660147597312\",\"medReqId\":\"329906419476205568\",\"quantity\":1.00,\"unitCode\":\"8\",\"dispenseQuantity\":1.00,\"lotNumber\":\"001-008\",\"preparerId\":\"322932010683207680\",\"prepareTime\":\"2026-06-29 19:43:25\",\"practitionerId\":\"308791351298756608\",\"dispenseTime\":\"2026-06-29 19:43:25\",\"dosageInstruction\":null,\"methodCode\":null,\"frequencyCode\":null,\"dose\":null,\"doseUnitCode\":null,\"summaryNo\":null,\"name\":\"注射用头孢呋辛钠\",\"merchandiseName\":\"注射用头孢呋辛钠\",\"categoryCode\":\"8\",\"totalVolume\":\"0.75g\",\"comprisedText\":null,\"ingredient\":null,\"approvalNumber\":null,\"skinTestFlag\":0,\"injectFlag\":1,\"manufacturerText\":\"山东润泽制药有限公司\",\"antibioticFlag\":0,\"rxFlag\":1,\"doseFormCode\":\"4\",\"packageUnitCode\":\"10\",\"pyStr\":\"ZSYTBFXN\"}],\"total\":1,\"size\":10,\"current\":1,\"pages\":1}}")))
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
            content = @Content(schema = @Schema(implementation = BatchDispenseResultJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"successCount\":0,\"failCount\":2,\"successIds\":[],\"failIds\":[\"329907418504892416\",\"329907418609750016\"],\"message\":\"发药完成：成功 0 条，失败 2 条\"}}")))
    })
    @PostMapping("/batch-dispense")
    @Override
    public JsonVO<BatchDispenseResultVO> batchDispense(
            @RequestBody @Valid BatchDispenseCommand command) {
        BatchDispenseResultVO result = drugService.batchDispense(command);
        return JsonVO.success(result);
    }
}
