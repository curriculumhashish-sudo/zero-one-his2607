package com.zeroone.star.configuration.converter;

import com.zeroone.star.configuration.entity.MedicalTemplateDO;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateCreateDTO;
import com.zeroone.star.project.dto.j5.emr.MedicalTemplateUpdateDTO;

/**
 * 病历模板接口模型与数据对象转换器。
 */
public final class MedicalTemplateConverter {

    private MedicalTemplateConverter() {
    }

    public static MedicalTemplateDO toDO(MedicalTemplateCreateDTO source) {
        MedicalTemplateDO target = new MedicalTemplateDO();
        target.setName(source.getName());
        target.setCode(source.getCode());
        target.setDocumentTypeCode(source.getDocumentTypeCode());
        target.setOrganizationId(source.getOrganizationId());
        target.setEncounterClassEnum(source.getEncounterClassEnum());
        target.setEncounterTypeEnum(source.getEncounterTypeEnum());
        target.setContent(source.getContent());
        target.setEnabled(source.getEnabled());
        target.setScopeLevel(source.getScopeLevel());
        target.setOwnerPractitionerId(source.getOwnerPractitionerId());
        target.setEncounterClassCode(source.getEncounterClassCode());
        return target;
    }

    public static MedicalTemplateDO toDO(MedicalTemplateUpdateDTO source) {
        MedicalTemplateDO target = new MedicalTemplateDO();
        target.setName(source.getName());
        target.setCode(source.getCode());
        target.setDocumentTypeCode(source.getDocumentTypeCode());
        target.setOrganizationId(source.getOrganizationId());
        target.setEncounterClassEnum(source.getEncounterClassEnum());
        target.setEncounterTypeEnum(source.getEncounterTypeEnum());
        target.setContent(source.getContent());
        target.setEnabled(source.getEnabled());
        target.setScopeLevel(source.getScopeLevel());
        target.setOwnerPractitionerId(source.getOwnerPractitionerId());
        target.setEncounterClassCode(source.getEncounterClassCode());
        return target;
    }
}
