package com.zeroone.star.workbench.service.impl;

import com.zeroone.star.project.vo.j2.workbench.WorkbenchMenuVO;
import com.zeroone.star.workbench.entity.WhaleMenus;
import org.mapstruct.Mapper;
import org.mapstruct.ReportingPolicy;

import java.util.List;

/**
 * <p>
 * 菜单实体与VO互转（MapStruct）
 * </p>
 * <p>注：工作台主查询用 MyBatis resultMap 直接映射，本 Mapper 保留用于其他单表 CRUD 场景</p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface WorkbenchMapper {

    WorkbenchMenuVO toVO(WhaleMenus entity);

    WhaleMenus toEntity(WorkbenchMenuVO vo);

    List<WorkbenchMenuVO> toVOList(List<WhaleMenus> entities);

    List<WhaleMenus> toEntityList(List<WorkbenchMenuVO> vos);
}