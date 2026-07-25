package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.MedMedicationRequest;
import org.apache.ibatis.annotations.Mapper;

/**
 * 药品处方请求 Mapper
 * 对应表：med_medication_request
 *
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface MedMedicationRequestMapper extends BaseMapper<MedMedicationRequest> {
}
