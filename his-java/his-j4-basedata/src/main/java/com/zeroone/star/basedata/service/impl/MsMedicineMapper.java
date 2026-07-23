package com.zeroone.star.basedata.service.impl;
import com.zeroone.star.basedata.entity.medicine.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.medicine.MedMedicationDefinitionDO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDetailDTO;
import com.zeroone.star.project.vo.j4.medicine.MedicineDetailVO;
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
     * @param  chargeDefinitionDO 收费项目定于实体
     * @return 药品详情DTO
     */
    @Mappings({
            @Mapping(target = "id", source = "medDefinitionDO.id"),
            @Mapping(target = "name", source = "medDefinitionDO.name"),
            @Mapping(target = "medVersion", source = "medDefinitionDO.medVersion"),
            @Mapping(target = "categoryCode", source = "medDefinitionDO.categoryCode"),
            @Mapping(target = "manufacturerText", source = "medDefinitionDO.manufacturerText"),
            @Mapping(target = "doseFormCode", source = "medDefinitionDO.doseFormCode"),
            @Mapping(target = "totalVolume", source = "medDefinitionDO.totalVolume"),
            @Mapping(target = "doseUnitCode", source = "medDefinitionDO.doseUnitCode"),
            @Mapping(target = "purchaseSplitRatio", source = "medDefinitionDO.purchaseUnitRatio"),
            @Mapping(target = "stockSplitRatio", source = "medDefinitionDO.stockUnitRatio"),
            @Mapping(target = "packageSplitRatio", source = "medDefinitionDO.packageUnitRatio"),
            @Mapping(target = "orgId", source = "medDefinitionDO.orgId"),
            @Mapping(target = "statusEnum", source = "medDefinitionDO.statusEnum"),
    })
    MedicineDetailDTO entityToDetailDTO(MedMedicationDefinitionDO medDefinitionDO,
                                        AdmChargeItemDefinitionDO chargeDefinitionDO);

    /**
     * <p>
     * 描述：将 药品详情DTO 转换为 药品详情VO
     * </p>
     *
     * @param  dto 药品详情DTO
     * @return 药品详情VO
     */
    MedicineDetailVO medicineDetailDtoToVo(MedicineDetailDTO dto);
}
