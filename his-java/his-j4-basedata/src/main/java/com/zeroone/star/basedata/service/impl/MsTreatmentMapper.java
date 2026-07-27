package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionChildDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentUpdateDTO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;
import com.zeroone.star.project.vo.j4.treatment.WkfActivityDefinitionChildVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;

import java.util.List;

@Mapper(componentModel = "spring")
public interface MsTreatmentMapper {

    @Mappings({
            @Mapping(target = "categoryCode_dictText", ignore = true),
            @Mapping(target = "retailPrice", ignore = true)
    })
    TreatmentSelectVO toSelectVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentSelectVO> toSelectVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    TreatmentSelectDTO toSelectDTO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentSelectDTO> toSelectDTOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(target = "price", ignore = true)
    })
    TreatmentDTO toDTO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentDTO> toDTOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(target = "categoryCode_dictText", ignore = true),
            @Mapping(target = "statusEnum_dictText", ignore = true),
            @Mapping(target = "typeEnum_dictText", ignore = true),
            @Mapping(target = "price", ignore = true)
    })
    TreatmentListVO toListVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentListVO> toListVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(source = "isDeleted", target = "deleteFlag"),
            @Mapping(target = "categoryCode_dictText", ignore = true),
            @Mapping(target = "packageUnitCode_dictText", ignore = true),
            @Mapping(target = "statusEnum_dictText", ignore = true),
            @Mapping(target = "typeEnum_dictText", ignore = true),
            @Mapping(target = "itemTypeCode_dictText", ignore = true),
            @Mapping(target = "ruleId_dictText", ignore = true),
            @Mapping(target = "ybType", ignore = true),
            @Mapping(target = "ybType_dictText", ignore = true),
            @Mapping(target = "itemTypeCode", ignore = true),
            @Mapping(target = "purchasePrice", ignore = true),
            @Mapping(target = "retailPrice", ignore = true),
            @Mapping(target = "maximumRetailPrice", ignore = true),
            @Mapping(target = "children", ignore = true)
    })
    TreatmentDetailVO toDetailVO(WkfActivityDefinitionDO entity);

    WkfActivityDefinitionChildVO toChildVO(WkfActivityDefinitionChildDO childDO);

    List<WkfActivityDefinitionChildVO> toChildVOList(List<WkfActivityDefinitionChildDO> childDOS);

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
    WkfActivityDefinitionDO updateDtoToEntity(TreatmentUpdateDTO dto);
}
