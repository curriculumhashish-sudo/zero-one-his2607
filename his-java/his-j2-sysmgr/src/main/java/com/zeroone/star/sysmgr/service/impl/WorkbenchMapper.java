package com.zeroone.star.sysmgr.service.impl;

import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO2;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentNameDTO;
import com.zeroone.star.sysmgr.entity.WhaleTenants;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;

import java.util.List;

@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface WorkbenchMapper {

    @Mapping(target = "createdAt", dateFormat = "yyyy-MM-dd HH:mm:ss")
    @Mapping(target = "updatedAt", dateFormat = "yyyy-MM-dd HH:mm:ss")
    RentDTO2 toRentDTO2(WhaleTenants entity);

    WhaleTenants toEntity(RentDTO2 dto);

    List<RentDTO2> toRentDTO2List(List<WhaleTenants> entities);

    RentNameDTO toRentNameDTO(WhaleTenants entity);

    List<RentNameDTO> toRentNameDTOList(List<WhaleTenants> entities);
}