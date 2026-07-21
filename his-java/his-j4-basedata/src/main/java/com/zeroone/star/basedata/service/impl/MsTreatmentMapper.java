package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.WkfActivityDefinitionDO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogAddDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogEditDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogSelectDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.ServiceCatalogUpdateDTO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogDetailVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogListVO;
import com.zeroone.star.project.vo.j4.treatment.ServiceCatalogSelectVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;

import java.util.List;

@Mapper(componentModel = "spring")
public interface MsTreatmentMapper {

    ServiceCatalogSelectVO toSelectVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<ServiceCatalogSelectVO> toSelectVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    ServiceCatalogSelectDTO toSelectDTO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<ServiceCatalogSelectDTO> toSelectDTOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(target = "statusText", ignore = true),
            @Mapping(target = "price", ignore = true)
    })
    TreatmentDTO toDTO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentDTO> toDTOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(target = "statusText", ignore = true),
            @Mapping(target = "price", ignore = true)
    })
    ServiceCatalogListVO toListVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<ServiceCatalogListVO> toListVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    ServiceCatalogDetailVO toDetailVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    WkfActivityDefinitionDO addDtoToEntity(ServiceCatalogAddDTO dto);

    WkfActivityDefinitionDO editDtoToEntity(ServiceCatalogEditDTO dto);

    WkfActivityDefinitionDO updateDtoToEntity(ServiceCatalogUpdateDTO dto);
}