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
            @Mapping(target = "price", ignore = true)
    })
    TreatmentListVO toListVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentListVO> toListVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(source = "isDeleted", target = "deleteFlag"),
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

    WkfActivityDefinitionDO addDtoToEntity(TreatmentAddDTO dto);

    WkfActivityDefinitionDO editDtoToEntity(TreatmentEditDTO dto);

    WkfActivityDefinitionDO updateDtoToEntity(TreatmentUpdateDTO dto);
}
