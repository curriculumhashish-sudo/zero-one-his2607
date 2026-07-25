package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.MedMedicationDefinition;
import org.apache.ibatis.annotations.Mapper;

/**
 * 药品定义 Mapper
 * 对应表：med_medication_definition
 *
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface MedMedicationDefinitionMapper extends BaseMapper<MedMedicationDefinition> {
}
