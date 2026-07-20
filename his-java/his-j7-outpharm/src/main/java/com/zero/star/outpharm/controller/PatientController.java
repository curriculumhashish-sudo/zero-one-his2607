package com.zero.star.outpharm.controller;

import com.zeroone.star.project.j7.outpharm.OutPharmApis;
import com.zeroone.star.project.query.j7.outpharm.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.outpharm.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientQueueVO;
import com.zero.star.outpharm.service.PatientService;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.RestController;
import com.baomidou.mybatisplus.core.metadata.IPage;

/**
 * 患者队列 Controller
 *
 * @author zeroone
 * @since 2026-07-xx
 */
@RestController
@Validated
@Tag(name = "门诊发药-患者队列", description = "患者队列相关接口")
@RequiredArgsConstructor
public class PatientController implements OutPharmApis {

    private final PatientService patientService;

    @Override
    public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(PatientQueueQuery query) {
        IPage<PatientQueueVO> page = patientService.pagePatientQueue(query);
        if (page == null || page.getRecords().isEmpty()) {
            return JsonVO.fail(null);
        }
        return JsonVO.success(page);
    }

    @Override
    public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(PatientDrugItemQuery query) {
        // 本 Controller 不实现药品接口
        return JsonVO.fail("请调用药品处方接口");
    }
}