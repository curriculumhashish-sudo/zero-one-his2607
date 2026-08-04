package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionChildDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;
import com.zeroone.star.project.vo.j4.treatment.WkfActivityDefinitionChildVO;
import org.mapstruct.BeanMapping;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;
import org.mapstruct.ReportingPolicy;

import java.util.List;

@Mapper(componentModel = "spring")
public interface MsTreatmentMapper {

    TreatmentSelectVO toSelectVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentSelectVO> toSelectVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(target = "price", ignore = true)
    })
    TreatmentListVO toListVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentListVO> toListVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(target = "ybType", ignore = true),
            @Mapping(target = "typeCode", ignore = true),
            @Mapping(target = "purchasePrice", ignore = true),
            @Mapping(target = "retailPrice", ignore = true),
            @Mapping(target = "maximumRetailPrice", ignore = true),
            @Mapping(target = "children", ignore = true)
    })
    TreatmentDetailVO toDetailVO(WkfActivityDefinitionDO entity);

    WkfActivityDefinitionChildVO toChildVO(WkfActivityDefinitionChildDO childDO);

    List<WkfActivityDefinitionChildVO> toChildVOList(List<WkfActivityDefinitionChildDO> childDOS);

    /**
     * AddDTO转主表DO：价格/财务类型/子项等关联表字段由Service层单独处理，不映射到主表DO
     */
    @BeanMapping(unmappedTargetPolicy = ReportingPolicy.IGNORE)
    @Mappings({
            @Mapping(target = "id", ignore = true),
            @Mapping(target = "tenantId", ignore = true),
            @Mapping(target = "createdAt", ignore = true),
            @Mapping(target = "updatedAt", ignore = true),
            @Mapping(target = "createdBy", ignore = true),
            @Mapping(target = "updatedBy", ignore = true),
            @Mapping(target = "isDeleted", ignore = true),
            @Mapping(target = "deletedAt", ignore = true),
            @Mapping(target = "deletedBy", ignore = true),
            @Mapping(target = "version", ignore = true),
            @Mapping(target = "childrenJson", ignore = true)
    })
    WkfActivityDefinitionDO addDtoToEntity(TreatmentAddDTO dto);

    /**
     * EditDTO转主表DO：价格/财务类型/子项等关联表字段由Service层单独处理，不映射到主表DO
     */
    @BeanMapping(unmappedTargetPolicy = ReportingPolicy.IGNORE)
    @Mappings({
            @Mapping(target = "tenantId", ignore = true),
            @Mapping(target = "createdAt", ignore = true),
            @Mapping(target = "updatedAt", ignore = true),
            @Mapping(target = "createdBy", ignore = true),
            @Mapping(target = "updatedBy", ignore = true),
            @Mapping(target = "isDeleted", ignore = true),
            @Mapping(target = "deletedAt", ignore = true),
            @Mapping(target = "deletedBy", ignore = true),
            @Mapping(target = "version", ignore = true),
            @Mapping(target = "childrenJson", ignore = true)
    })
    WkfActivityDefinitionDO editDtoToEntity(TreatmentEditDTO dto);

    /**
     * 子项VO转DO（新增/编辑子项记录时使用）
     */
    @Mappings({
            @Mapping(target = "id", ignore = true),
            @Mapping(target = "tenantId", ignore = true),
            @Mapping(target = "createdAt", ignore = true),
            @Mapping(target = "updatedAt", ignore = true),
            @Mapping(target = "createdBy", ignore = true),
            @Mapping(target = "updatedBy", ignore = true),
            @Mapping(target = "isDeleted", ignore = true),
            @Mapping(target = "deletedAt", ignore = true),
            @Mapping(target = "deletedBy", ignore = true),
            @Mapping(target = "version", ignore = true),
            @Mapping(target = "activityId", ignore = true)
    })
    WkfActivityDefinitionChildDO childVoToEntity(WkfActivityDefinitionChildVO childVO);

    List<WkfActivityDefinitionChildDO> childVoListToEntityList(List<WkfActivityDefinitionChildVO> childVOs);
}
