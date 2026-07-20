package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.TreatmentDO;
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

    TreatmentSelectVO toSelectVO(TreatmentDO treatmentDO);

    List<TreatmentSelectVO> toSelectVOList(List<TreatmentDO> treatmentDOS);

    TreatmentSelectDTO toSelectDTO(TreatmentDO treatmentDO);

    List<TreatmentSelectDTO> toSelectDTOList(List<TreatmentDO> treatmentDOS);

    @Mappings({
            @Mapping(target = "statusText", ignore = true),
            @Mapping(target = "price", ignore = true)
    })
    TreatmentDTO toDTO(TreatmentDO treatmentDO);

    List<TreatmentDTO> toDTOList(List<TreatmentDO> treatmentDOS);

    @Mappings({
            @Mapping(target = "statusText", ignore = true),
            @Mapping(target = "price", ignore = true)
    })
    TreatmentListVO toListVO(TreatmentDO treatmentDO);

    List<TreatmentListVO> toListVOList(List<TreatmentDO> treatmentDOS);

    TreatmentDetailVO toDetailVO(TreatmentDO treatmentDO);

    TreatmentDO addDtoToEntity(TreatmentAddDTO dto);

    TreatmentDO editDtoToEntity(TreatmentEditDTO dto);

    TreatmentDO updateDtoToEntity(TreatmentUpdateDTO dto);
}