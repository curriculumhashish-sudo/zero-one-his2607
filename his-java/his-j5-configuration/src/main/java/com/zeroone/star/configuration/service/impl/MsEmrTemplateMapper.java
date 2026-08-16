package com.zeroone.star.configuration.service.impl;

import com.zeroone.star.configuration.entity.EmrTemplateDO;
import com.zeroone.star.project.vo.j5.emr.MedicalTemplateDetailVO;
import com.zeroone.star.project.vo.j5.emr.MedicalTemplateVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;
import org.mapstruct.factory.Mappers;

/**
 * 病历模板实体与 VO 之间的 MapStruct 转换器
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsEmrTemplateMapper {

    /**
     * DO → 列表VO
     * name → templateName, Boolean isEnabled → Integer isEnabled
     */
    @Mapping(source = "name", target = "templateName")
    @Mapping(target = "isEnabled", expression = "java(entity.getIsEnabled() != null ? (entity.getIsEnabled() ? 1 : 0) : null)")
    MedicalTemplateVO toListVO(EmrTemplateDO entity);

    /**
     * DO → 详情VO
     * name → templateName, content → contentJson, Boolean isEnabled → Integer isEnabled
     */
    @Mapping(source = "name", target = "templateName")
    @Mapping(source = "content", target = "contentJson")
    @Mapping(target = "isEnabled", expression = "java(entity.getIsEnabled() != null ? (entity.getIsEnabled() ? 1 : 0) : null)")
    MedicalTemplateDetailVO toDetailVO(EmrTemplateDO entity);
}
