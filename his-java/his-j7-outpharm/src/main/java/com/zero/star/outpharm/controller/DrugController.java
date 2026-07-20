package com.zero.star.outpharm.controller;

import com.zeroone.star.project.j7.outpharm.OutPharmApis;
import com.zeroone.star.project.query.j7.outpharm.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.outpharm.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientDrugItemVO;
import com.zero.star.outpharm.service.DrugService;
import com.zeroone.star.project.vo.j7.outpharm.PatientQueueVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.RestController;
import com.baomidou.mybatisplus.core.metadata.IPage;

/**
 * 药品发药 Controller
 *
 * @author zeroone
 * @since 2026-07-xx
 */
@RestController
@Validated
@Tag(name = "门诊发药-药品处方", description = "处方药品项目相关接口")
@RequiredArgsConstructor
public class DrugController implements OutPharmApis {

    private final DrugService drugService;

    @Override
    public JsonVO<IPage<PatientQueueVO>> pagePatientQueue(PatientQueueQuery query) {
        // 本 Controller 不实现患者队列接口
        return JsonVO.fail("请调用患者队列接口");
    }

    @Override
    public JsonVO<IPage<PatientDrugItemVO>> pagePatientDrugItems(PatientDrugItemQuery query) {
        IPage<PatientDrugItemVO> page = drugService.pagePatientDrugItems(query);
        if (page == null || page.getRecords().isEmpty()) {
            return JsonVO.fail(null);
        }
        return JsonVO.success(page);
    }
}