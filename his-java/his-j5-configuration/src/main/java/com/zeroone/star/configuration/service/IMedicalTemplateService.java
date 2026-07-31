package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.j5.emr.MedicalTemplateCreateDTO;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateUpdateDTO;

/**
 * 病历模板写操作服务。
 */
public interface IMedicalTemplateService {

    String addMedicalTemplate(MedicalTemplateCreateDTO dto);

    String modifyMedicalTemplate(String id, MedicalTemplateUpdateDTO dto);

    void removeMedicalTemplate(String id);
}
