package com.zeroone.star.configuration.controller.template;

import com.zeroone.star.configuration.service.IMedicalTemplateService;
import com.zeroone.star.project.j5.emr.template.MedicalTemplateWriteApis;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateCreateDTO;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.web.bind.annotation.RestController;

/**
 * 病历模板写操作控制器。
 */
@RestController
public class MedicalTemplateWriteController implements MedicalTemplateWriteApis {

    private final IMedicalTemplateService medicalTemplateService;

    public MedicalTemplateWriteController(IMedicalTemplateService medicalTemplateService) {
        this.medicalTemplateService = medicalTemplateService;
    }

    @Override
    public JsonVO<String> addMedicalTemplate(MedicalTemplateCreateDTO dto) {
        try {
            return JsonVO.success(medicalTemplateService.addMedicalTemplate(dto));
        } catch (IllegalArgumentException ex) {
            return JsonVO.fail(ex.getMessage());
        }
    }

    @Override
    public JsonVO<String> modifyMedicalTemplate(String id, MedicalTemplateUpdateDTO dto) {
        try {
            return JsonVO.success(medicalTemplateService.modifyMedicalTemplate(id, dto));
        } catch (IllegalArgumentException ex) {
            return JsonVO.fail(ex.getMessage());
        }
    }

    @Override
    public JsonVO<String> removeMedicalTemplate(String id) {
        try {
            medicalTemplateService.removeMedicalTemplate(id);
            return JsonVO.success(id);
        } catch (IllegalArgumentException ex) {
            return JsonVO.fail(ex.getMessage());
        }
    }
}
