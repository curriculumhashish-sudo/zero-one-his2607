package com.zeroone.star.basedata.service.impl;
import com.zeroone.star.basedata.entity.Medicine.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.Medicine.MedMedicationDefinitionDO;
import com.zeroone.star.project.dto.j4.medicine.MedicineUpdateDTO;
import org.mapstruct.Mapper;
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
     * 描述：将 数据库实体 转换为 药品更新DTO
     * </p>
     *
     * @param  medDefinitionDO 药品定义实体
     * @param  chargeDefinitionDO 收费项目定于实体
     * @return 器材实体
     */
    MedicineUpdateDTO entityToUpdateDTO(MedMedicationDefinitionDO medDefinitionDO,
                                        AdmChargeItemDefinitionDO chargeDefinitionDO);


}
