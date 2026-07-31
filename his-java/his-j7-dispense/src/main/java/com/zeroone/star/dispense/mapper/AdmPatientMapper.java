package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.AdmPatient;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

@Mapper
public interface AdmPatientMapper extends BaseMapper<AdmPatient> {

    IPage<PatientQueueVO> pagePatientQueue(
            Page<?> page,
            @Param("query") PatientQueueQuery query
    );
}
