package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.MedMedicationDefinition;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import feign.Param;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 药品定义 Mapper
 * </p>
 *
 * @author j7
 * @since 2026-07-24
 */
@Mapper
public interface MedMedicationDefinitionMapper extends BaseMapper<MedMedicationDefinition> {

    IPage<PatientDrugItemVO> pagePatientDrugItems(
                Page<?> page,
                @Param("query") PatientDrugItemQuery query
        );

}
