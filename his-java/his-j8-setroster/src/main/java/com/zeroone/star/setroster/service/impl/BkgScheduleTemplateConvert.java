package com.zeroone.star.setroster.service.impl;

import com.zeroone.star.project.dto.j8.setroster.AddDailyScheduleDTO;
import com.zeroone.star.project.dto.j8.setroster.DailyScheduleDTO;
import com.zeroone.star.project.vo.j8.setroster.DailyScheduleOptionVO;
import com.zeroone.star.setroster.entity.BkgScheduleTemplate;
import org.mapstruct.*;

@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface BkgScheduleTemplateConvert {
    /**
     * 映射BkgScheduleTemplate 到 DailyScheduleDTO
     * @param entity 待映射的DTO
     * @return 映射后的实体
     */
    DailyScheduleDTO toDto(BkgScheduleTemplate entity);

    /**
     * 映射BkgScheduleTemplate 到 DailyScheduleOptionVO
     * @param entity 待映射的DTO
     * @return 映射后的实体
     */
    DailyScheduleOptionVO toOptionVo(BkgScheduleTemplate entity);

    /**
     * 映射DailyScheduleDTO 到 BkgScheduleTemplate
     * @param dto 待映射的实体
     * @return 映射后的DTO
     */
    BkgScheduleTemplate toEntity(AddDailyScheduleDTO dto);

    /**
     * 更新BkgScheduleTemplate
     * @param dto 待映射的实体
     * @param entity 待映射的DTO
     */
    @BeanMapping(nullValuePropertyMappingStrategy = NullValuePropertyMappingStrategy.IGNORE)
    void updateEntity(DailyScheduleDTO dto, @MappingTarget BkgScheduleTemplate entity);
}
