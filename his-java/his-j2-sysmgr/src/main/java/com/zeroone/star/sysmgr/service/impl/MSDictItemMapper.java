package com.zeroone.star.sysmgr.service.impl;

import com.zeroone.star.project.dto.j2.sysmgr.DictItemUpdateDTO;
import com.zeroone.star.sysmgr.entity.TerConcept;
import org.mapstruct.Mapper;
import org.mapstruct.MappingTarget;
import org.mapstruct.ReportingPolicy;

/**
 * <p>
 * 字典项 MapStruct 转换器
 * </p>
 *
 * @author zeroone
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MSDictItemMapper {

    /**
     * 将更新DTO的值映射到实体（只更新DTO有的字段）
     * @param dto 更新DTO
     * @param entity 目标实体
     */
    void updateDtoToEntity(DictItemUpdateDTO dto, @MappingTarget TerConcept entity);

}
