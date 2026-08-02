package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.medicine.MedMedicationDefinitionDO;
import com.zeroone.star.project.dto.j4.medicine.MedicineAddDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDetailDTO;
import com.zeroone.star.project.vo.j4.medicine.MedicineDetailVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineListVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;
import org.mapstruct.ReportingPolicy;

/**
 * <p>
 * 描述：药品管理对应MapStruct映射接口
 * </p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper(componentModel = "spring",unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsMedicineMapper {
    /**
     * <p>
     * 描述：将 数据库实体 转换为 药品详情DTO
     * </p>
     *
     * @param  medDefinitionDO 药品定义实体
     * @return 药品详情DTO
     */


    MedicineDetailDTO entityToDetailDTO(MedMedicationDefinitionDO medDefinitionDO);

    /**
     * <p>
     * 描述：将 药品详情DTO 转换为 数据库实体
     * </p>
     *
     * @param  dto 药品详情DTO
     * @return 药品定义实体
     */
    MedMedicationDefinitionDO detailDtoToEntity(MedicineDetailDTO dto);

    /**
     * <p>
     * 描述：将 药品详情DTO 转换为 药品详情VO
     * </p>
     *
     * @param  dto 药品详情DTO
     * @return 药品详情VO
     */

    MedicineDetailVO medicineDetailDtoToVo(MedicineDetailDTO dto);

    /**
     * 新增药品DTO转换为药品实体。
     */
    @Mapping(source = "definition", target = "medicationdefinition")
    MedMedicationDefinitionDO addDtoToEntity(MedicineAddDTO dto);

    /**
     * 药品实体转换为药品列表VO。
     */
    MedicineListVO entityToListVO(MedMedicationDefinitionDO entity);
}
