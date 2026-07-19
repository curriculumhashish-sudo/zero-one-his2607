package com.zeroone.star.basedata.assembler;

import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDetailDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;

import java.util.List;

@Mapper(componentModel = "spring")
public interface MsTreatmentMapper {

    TreatmentSelectDTO toSelectDTO(Treatment treatment);

    List<TreatmentSelectDTO> toSelectDTOList(List<Treatment> treatments);

    @Mappings({
            @Mapping(target = "statusText", ignore = true),
            @Mapping(target = "price", ignore = true)
    })
    TreatmentDTO toDTO(Treatment treatment);

    List<TreatmentDTO> toDTOList(List<Treatment> treatments);

    @Mappings({
            @Mapping(target = "statusText", ignore = true),
            @Mapping(target = "typeName", ignore = true),
            @Mapping(target = "price", ignore = true)
    })
    TreatmentDetailDTO toDetailDTO(Treatment treatment);
}
