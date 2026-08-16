package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.sample.AdmSpecimenDefinitionDO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionAddDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionUpdateDTO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenOptionVO;
import org.mapstruct.Mapper;
import org.mapstruct.ReportingPolicy;

import java.util.List;

/**
 * <p>
 * 描述：样本管理对应MapStruct映射接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsSampleMapper {

    /**
     * 将新增 DTO 映射为 数据库实体
     *
     * @param dto 新增参数
     * @return 样本实体
     */
    AdmSpecimenDefinitionDO saveDtoToEntity(SpecimenDefinitionAddDTO dto);

    /**
     * 将修改 DTO 映射为 数据库实体
     *
     * @param dto 修改参数
     * @return 样本实体
     */
    AdmSpecimenDefinitionDO updateDtoToEntity(SpecimenDefinitionUpdateDTO dto);

    /**
     * 将数据库实体 映射为 详情/列表展示 VO
     *
     * @param entity 样本实体
     * @return 样本展示VO
     */
    SpecimenDefinitionDetailVO entityToVo(AdmSpecimenDefinitionDO entity);

    /**
     * 将数据库实体列表 映射为 详情/列表展示 VO 列表
     *
     * @param entities 样本实体列表
     * @return 样本展示VO列表
     */
    List<SpecimenDefinitionDetailVO> entitiesToVos(List<AdmSpecimenDefinitionDO> entities);

    /**
     * 将单个数据库实体 映射为 下拉选项 VO
     *
     * @param entity 样本实体
     * @return 下拉选项VO
     */
    SpecimenOptionVO entityToOptionVo(AdmSpecimenDefinitionDO entity);
}
