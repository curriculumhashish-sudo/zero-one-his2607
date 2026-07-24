package com.zeroone.star.configuration.service.impl;

import com.zeroone.star.configuration.entity.EmrTemplateDO;
import com.zeroone.star.project.j5.emr.template.vo.MedicalTemplateDetailVO;
import com.zeroone.star.project.j5.emr.template.vo.MedicalTemplateVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;

/**
 * 病历模板实体与 VO 之间的 MapStruct 转换器
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsEmrTemplateMapper {

    /**
     * DO → 列表VO
     * name → templateName, content → templateContent
     */
    @Mapping(source = "name", target = "templateName")
    @Mapping(source = "content", target = "templateContent")
    MedicalTemplateVO toListVO(EmrTemplateDO entity);

    /**
     * DO → 详情VO
     * name → templateName, content → templateContent
     */
    @Mapping(source = "name", target = "templateName")
    @Mapping(source = "content", target = "templateContent")
    MedicalTemplateDetailVO toDetailVO(EmrTemplateDO entity);
}
