package com.zeroone.star.project.j7.outpharm;

import com.zeroone.star.project.dto.j7.dispense.BatchDispenseCommand;
import com.zeroone.star.project.vo.j7.dispense.BatchDispenseResultVO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.baomidou.mybatisplus.core.metadata.IPage;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import jakarta.validation.Valid;

/**
 * 门诊发药模块接口
 */
@Tag(name = "门诊发药", description = "门诊发药相关接口（患者队列、处方药品查询）")
@Validated
public interface OutPharmApis {

    /**
     * 分页查询患者队列
     */
    @Operation(
            summary = "分页查询患者队列",
            description = "根据姓名、状态、机构等条件分页查询候诊/发药患者队列"
    )
    @GetMapping("/api/j7/outpharm/patient-queue")
    JsonVO<IPage<PatientQueueVO>> pagePatientQueue(
            @Parameter(hidden = true)
            PatientQueueQuery query
    );

    /**
     * 分页查询患者药品项目（处方明细 + 药品信息）
     */
    @Operation(
            summary = "分页查询患者药品项目",
            description = "根据患者ID、就诊ID、处方状态等条件查询患者处方药品明细"
    )
    @GetMapping("/api/j7/outpharm/patient-drug-items")
    JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(
            @Parameter(hidden = true)
            PatientDrugItemQuery query
    );

    /**
     * 批量发药
     */
    @Operation(
            summary = "批量发药",
            description = "根据发药记录ID列表批量确认发药，将 PENDING 状态的门诊发药记录更新为 COMPLETED"
    )
    @PostMapping("/api/j7/outpharm/batch-dispense")
    JsonVO<BatchDispenseResultVO> batchDispense(
            @RequestBody @Valid BatchDispenseCommand command
    );
}