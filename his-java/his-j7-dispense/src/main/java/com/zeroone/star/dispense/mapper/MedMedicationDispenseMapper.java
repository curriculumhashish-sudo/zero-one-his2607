package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.MedMedicationDispense;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
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

    /**
     * 查询门诊退药患者分页列表
     * @param page 分页参数
     * @param query 查询条件
     * @return 患者队列分页数据
     */
    IPage<PatientQueueVO> selectRefundPatientPage(Page<PatientQueueVO> page,
                                                  @Param("query") PatientQueueQuery query);

    /**
     * 分页查询门诊退药明细列表
     * @param page 分页参数
     * @param query 查询条件
     * @return 药品明细分页数据
     */
    IPage<PatientDrugItemVO> selectRefundDrugItems(Page<PatientDrugItemVO> page,
                                                   @Param("query") PatientDrugItemQuery query);
}
