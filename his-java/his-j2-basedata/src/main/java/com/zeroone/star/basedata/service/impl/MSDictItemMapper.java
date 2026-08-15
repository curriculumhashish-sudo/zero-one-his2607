package com.zeroone.star.basedata.service.impl;


import com.zeroone.star.basedata.entity.TerConcept;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictItemUpdateDTO;
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
