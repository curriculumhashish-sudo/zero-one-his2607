package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.common.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.equipment.AdmDeviceDefinitionDO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentAddDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentCategoryVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;

import java.util.List;

/**
 *
 * <p>
 * 描述：器材管理对应MapStruct映射接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsEquipmentMapper {

    /**
     * <p>
     * 描述：将器材添加DTO转换为实体
     * </p>
     *
     * @param equipmentAddDTO 新增器材DTO
     * @return 器材实体
     */
    AdmDeviceDefinitionDO saveDtoToEntity(EquipmentAddDTO equipmentAddDTO);

    /**
     * 将新增器材DTO转换为AdmChargeItemDefinitionDO实体类
     * @param equipmentAddDTO 新增器材DTO
     * @return 物品价格实体
     */
    AdmChargeItemDefinitionDO saveDtoToChargeItemDefinitionEntity(EquipmentAddDTO equipmentAddDTO);

    /**
     * <p>
     * 描述：将器材更新DTO转换为实体
     * </p>
     *
     * @param equipmentUpdateDTO 更新器材DTo
     * @return 器材实体
     */
    AdmDeviceDefinitionDO updateDtoToEntity(EquipmentUpdateDTO equipmentUpdateDTO);

    /**
     * <p>
     * 描述：将器材实体转换为VO
     * </p>
     *
     * @param equipment 器材实体
     * @return 器材VO
     */
    EquipmentVO entityToVo(AdmDeviceDefinitionDO equipment);

    /**
     * <p>
     * 描述：将器材实体列表转换为VO列表
     * </p>
     *
     * @param equipmentList 器材实体列表
     * @return 器材VO列表
     */
    List<EquipmentVO> entityListToVoList(List<AdmDeviceDefinitionDO> equipmentList);

    /**
     * 将单个数据库实体映射为下拉选项 VO
     *
     * @param entity 器材实体
     * @return 下拉选项VO
     */
    @Mapping(source = "categoryCode", target = "value")
    @Mapping(source = "name", target = "label")
    EquipmentCategoryVO entityToOptionVo(AdmDeviceDefinitionDO entity);

    /**
     * 将数据库实体列表映射为下拉选项 VO 列表
     *
     * @param entities 器材实体列表
     * @return 下拉选项VO列表
     */
    List<EquipmentCategoryVO> entitiesToOptionVos(List<AdmDeviceDefinitionDO> entities);

}
