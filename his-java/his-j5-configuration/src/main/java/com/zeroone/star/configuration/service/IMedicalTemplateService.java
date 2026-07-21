package com.zeroone.star.configuration.service;

import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateCreateDTO;
import com.zeroone.star.project.j5.emr.template.dto.MedicalTemplateUpdateDTO;

/**
 * 病历模板写操作服务。
 */
public interface IMedicalTemplateService {

    String addMedicalTemplate(MedicalTemplateCreateDTO dto);

    String modifyMedicalTemplate(String id, MedicalTemplateUpdateDTO dto);

    void removeMedicalTemplate(String id);
}
