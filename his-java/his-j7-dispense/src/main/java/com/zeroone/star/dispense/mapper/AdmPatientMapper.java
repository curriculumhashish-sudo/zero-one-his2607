package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.AdmPatient;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
import feign.Param;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 患者 Mapper
 * </p>
 *
 * @author j7
 * @since 2026-07-24
 */
@Mapper
public interface AdmPatientMapper extends BaseMapper<AdmPatient> {
    IPage<PatientQueueVO> pagePatientQueue(
            Page<?> page,
            @Param("query") PatientQueueQuery query
    );
}
