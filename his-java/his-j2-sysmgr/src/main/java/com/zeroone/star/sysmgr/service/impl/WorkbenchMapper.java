package com.zeroone.star.sysmgr.service.impl;

import com.zeroone.star.project.dto.j2.sysmgr.rent.RentDTO2;
import com.zeroone.star.project.dto.j2.sysmgr.rent.RentNameDTO;
import com.zeroone.star.sysmgr.entity.WhaleTenants;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;

import java.util.List;

/**
 * <p>
 * 描述：租户模块对象转换器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface WorkbenchMapper {


    RentDTO2 toRentDTO2(WhaleTenants entity);

    WhaleTenants toEntity(RentDTO2 dto);

    List<RentDTO2> toRentDTO2List(List<WhaleTenants> entities);

    RentNameDTO toRentNameDTO(WhaleTenants entity);

    List<RentNameDTO> toRentNameDTOList(List<WhaleTenants> entities);
}