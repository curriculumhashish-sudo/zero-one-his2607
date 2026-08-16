package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.IMedicalTemplateService;
import com.zeroone.star.project.j5.emr.MedicalTemplateWriteApis;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateCreateDTO;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.RestController;

/**
 * 病历模板写操作控制器。
 */
@RestController
@Validated
@Tag(name = "配置管理-病历模板", description = "病历模板配置管理接口")
public class MedicalTemplateWriteController implements MedicalTemplateWriteApis {

    private final IMedicalTemplateService medicalTemplateService;

    public MedicalTemplateWriteController(IMedicalTemplateService medicalTemplateService) {
        this.medicalTemplateService = medicalTemplateService;
    }

    @Override
    public JsonVO<String> addMedicalTemplate(MedicalTemplateCreateDTO dto) {
        return JsonVO.success(medicalTemplateService.addMedicalTemplate(dto));
    }

    @Override
    public JsonVO<String> modifyMedicalTemplate(String id, MedicalTemplateUpdateDTO dto) {
        return JsonVO.success(medicalTemplateService.modifyMedicalTemplate(id, dto));
    }

    @Override
    public JsonVO<String> removeMedicalTemplate(String id) {
        medicalTemplateService.removeMedicalTemplate(id);
        return JsonVO.success(id);
    }
}
