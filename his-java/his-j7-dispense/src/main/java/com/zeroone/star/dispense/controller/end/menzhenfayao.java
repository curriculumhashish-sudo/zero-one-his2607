package com.zeroone.star.dispense.controller.end;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.dispense.service.DrugService;
import com.zeroone.star.dispense.service.PatientService;
import com.zeroone.star.project.j7.DispenseApis;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@Tag(name = "门诊发药", description = "门诊发药基础流程")
public class menzhenfayao implements DispenseApis {


    /**
     * 等待幸福controller层部分
     */
    @Resource
    private PatientService patientService;
    @Resource
    private DrugService drugService;

    @Operation(
            summary = "分页查询患者队列",
            description = "根据姓名、状态、机构等条件分页查询候诊/发药患者队列"
    )
    @GetMapping("/patient-queue")
    @Override
    public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(PatientQueueQuery query) {
        IPage<PatientQueueVO> page = patientService.pagePatientQueue(query);
        return page == null || page.getRecords().isEmpty()
                ? JsonVO.fail(null)
                : JsonVO.success(page);
    }

    @Operation(
            summary = "分页查询患者药品项目",
            description = "根据患者ID、就诊ID、处方状态等条件查询患者处方药品明细"
    )
    @GetMapping("/patient-drug-items")
    @Override
    public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(PatientDrugItemQuery query) {
        IPage<PatientDrugItemVO> page = drugService.pagePatientDrugItems(query);
        return page == null || page.getRecords().isEmpty()
                ? JsonVO.fail(null)
                : JsonVO.success(page);
    }


    /**
     *
     */
}
