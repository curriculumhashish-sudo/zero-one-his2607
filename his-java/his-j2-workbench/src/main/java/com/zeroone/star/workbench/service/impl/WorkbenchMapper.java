package com.zeroone.star.workbench.service.impl;


import com.zeroone.star.project.vo.j2.workbench.WorkbenchMenuVO;
import com.zeroone.star.workbench.entity.WhaleMenus;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;

import java.util.List;

@Mapper(componentModel="spring",unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface WorkbenchMapper {
    @Mapping(source = "displayName", target = "display_name")
    @Mapping(source = "permissionName", target = "permission_name")
    @Mapping(source = "sortOrder", target = "sort_order")
    WorkbenchMenuVO toVO(WhaleMenus entity);

    @Mapping(source = "display_name", target = "displayName")
    @Mapping(source = "permission_name", target = "permissionName")
    @Mapping(source = "sort_order", target = "sortOrder")
    WhaleMenus toEntity(WorkbenchMenuVO vo);

    List<WorkbenchMenuVO> toVOList(List<WhaleMenus> entities);

    List<WhaleMenus> toEntityList(List<WorkbenchMenuVO> vos);
}