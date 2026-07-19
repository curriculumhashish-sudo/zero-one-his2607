package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDetailDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentOptionDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import com.zeroone.star.project.j4.basedata.TreatmentApis;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/basedata/treatment")
@Tag(name = "基于诊疗管理接口")
public class TreatmentController implements TreatmentApis {

    @Resource
    private ITreatmentService treatmentService;

    @Override
    @GetMapping("/query-treatment-list")
    public JsonVO<PageDTO<TreatmentDTO>> queryTreatmentList(TreatmentQuery query) {
        return JsonVO.success(treatmentService.queryTreatmentList(query));
    }

    @Override
    @GetMapping("/list-treatment-options")
    public JsonVO<PageDTO<TreatmentSelectDTO>> listTreatmentOptions(TreatmentSelectQuery query) {
        return JsonVO.success(treatmentService.listTreatmentOptions(query));
    }

    @Override
    @GetMapping("/get-treatment")
    public JsonVO<TreatmentDetailDTO> getTreatment(String id) {
        return JsonVO.success(treatmentService.getTreatmentById(id));
    }

    @Override
    @GetMapping("/list-category-options")
    public JsonVO<List<TreatmentOptionDTO>> listCategoryOptions() {
        return JsonVO.success(treatmentService.listCategoryOptions());
    }
}
