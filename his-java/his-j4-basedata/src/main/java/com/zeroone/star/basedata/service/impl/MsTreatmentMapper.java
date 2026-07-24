package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentUpdateDTO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;
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

    TreatmentDetailVO toDetailVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    WkfActivityDefinitionDO addDtoToEntity(TreatmentAddDTO dto);

    WkfActivityDefinitionDO editDtoToEntity(TreatmentEditDTO dto);

    WkfActivityDefinitionDO updateDtoToEntity(TreatmentUpdateDTO dto);
}