package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.MedMedicationDispense;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import java.util.List;

/**
 * <p>
 * 药品发放记录 Mapper
 * </p>
 *
 * @author j7
 * @since 2026-07-24
 */
@Mapper
public interface MedMedicationDispenseMapper extends BaseMapper<MedMedicationDispense> {
    IPage<DispensingSummaryVO> selectSummaryPage(Page<DispensingSummaryVO> page,
                                                 @Param("summaryNo") String summaryNo,
                                                 @Param("wardId") String wardId,
                                                 @Param("wardName") String wardName,
                                                 @Param("status") String status,
                                                 @Param("createTimeStart") String createTimeStart,
                                                 @Param("createTimeEnd") String createTimeEnd);

    List<PatientMedicationVO> selectPatientMedications(@Param("summaryNo") String summaryNo);
}
