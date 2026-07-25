package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.MedMedicationDispense;
import org.apache.ibatis.annotations.Mapper;

/**
 * 药品发药 Mapper
 * 对应表：med_medication_dispense
 *
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface MedMedicationDispenseMapper extends BaseMapper<MedMedicationDispense> {
}
