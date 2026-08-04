package com.zeroone.star.setroster.convert;

import com.zeroone.star.project.dto.j8.setroster.AddWeekScheduleTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.WeekScheduleTemplateDTO;
import com.zeroone.star.project.dto.j8.setroster.WeekScheduleTemplateItemDTO;
import com.zeroone.star.project.vo.j8.setroster.WeekScheduleTemplateDetailVO;
import com.zeroone.star.setroster.entity.WeekScheduleTemplate;
import com.zeroone.star.setroster.entity.WeekScheduleTemplateItem;
import org.mapstruct.*;

/**
 * <p>
 * 描述：周排班模板 MapStruct 转换接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface WeekScheduleTemplateConvert {

    /**
     * DO → DTO
     */
    WeekScheduleTemplateDTO toDTO(WeekScheduleTemplate entity);

    /**
     * AddDTO → DO（新增）
     */
    WeekScheduleTemplate toEntity(AddWeekScheduleTemplateDTO dto);

    /**
     * EditDTO → DO（修改）
     */
    @BeanMapping(nullValuePropertyMappingStrategy = NullValuePropertyMappingStrategy.IGNORE)
    void updateEntity(WeekScheduleTemplateDTO dto, @MappingTarget WeekScheduleTemplate entity);

    /**
     * DO → DetailVO
     */
    WeekScheduleTemplateDetailVO toDetailVO(WeekScheduleTemplate entity);

    /**
     * ItemDO → ItemDTO
     */
    WeekScheduleTemplateItemDTO toItemDTO(WeekScheduleTemplateItem entity);

    /**
     * ItemDTO → ItemDO（保留id，用于方案B逐条对比）
     */
    WeekScheduleTemplateItem toItemEntity(WeekScheduleTemplateItemDTO dto);

    /**
     * ItemDO → DetailItemVO（详情VO中的明细项）
     */
    WeekScheduleTemplateDetailVO.DetailItemVO toDetailItemVO(WeekScheduleTemplateItem entity);
}
