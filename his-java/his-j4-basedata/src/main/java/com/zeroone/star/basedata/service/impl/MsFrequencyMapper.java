package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.project.dto.j4.frequency.FrequencySaveDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyUpdateDTO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyOptionVO;
import com.zeroone.star.basedata.entity.frequency.CusFrequencyDO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;
import java.util.List;

/**
 * <p>
 * 描述：频次管理对应MapStruct映射接口
 * </p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsFrequencyMapper {

    /**
     * 将新增 DTO 映射为 数据库实体
     * @param dto 新增参数
     * @return 频次实体
     */
    CusFrequencyDO saveDtoToEntity(FrequencySaveDTO dto);

    /**
     * 将修改 DTO 映射为 数据库实体
     * @param dto 修改参数
     * @return 频次实体
     */
    CusFrequencyDO updateDtoToEntity(FrequencyUpdateDTO dto);

    /**
     * 将数据库实体 映射为 详情/列表展示 VO
     * @param entity 频次实体
     * @return 频次展现VO
     */
    FrequencyVO entityToVo(CusFrequencyDO entity);

    /**
     * 将数据库实体列表 映射为 详情/列表展示 VO 列表（用于分页/条件查询）
     * @param entities 频次实体列表
     * @return 频次展现VO列表
     */
    List<FrequencyVO> entitiesToVos(List<CusFrequencyDO> entities);

    /**
     * 将单个数据库实体 映射为 下拉选项 VO（特殊映射字段名）
     * @param entity 频次实体
     * @return 下拉选项VO
     */
    @Mapping(source = "sortNo", target = "sort")
    @Mapping(source = "rateCode", target = "value")
    @Mapping(source = "name", target = "label")
    FrequencyOptionVO entityToOptionVo(CusFrequencyDO entity);

    /**
     * 将数据库实体列表 映射为 下拉选项 VO 列表（用于获取频次名称大字典接口）
     * @param entities 频次实体列表
     * @return 下拉选项VO列表
     */
    List<FrequencyOptionVO> entitiesToOptionVos(List<CusFrequencyDO> entities);
}
