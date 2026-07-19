package com.zeroone.star.basedata.assembler;

import com.zeroone.star.basedata.entity.Treatment;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;

import java.util.List;

@Mapper(componentModel = "spring")
public interface MsTreatmentMapper {

    TreatmentSelectVO toSelectVO(Treatment treatment);

    List<TreatmentSelectVO> toSelectVOList(List<Treatment> treatments);

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
            @Mapping(target = "price", ignore = true)
    })
    TreatmentListVO toListVO(Treatment treatment);

    List<TreatmentListVO> toListVOList(List<Treatment> treatments);

    TreatmentDetailVO toDetailVO(Treatment treatment);
}
