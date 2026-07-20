package com.zeroone.star.project.j7.outpharm.controller;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j7.outpharm.PatientDrugItemDTO;
import com.zeroone.star.project.dto.j7.outpharm.PatientQueueDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.outpharm.PatientQueueVO;

/**
 * 门诊发药模块接口
 */
public interface OutPharmApis {

    /** 获取患者队列（条件+分页） */
    JsonVO<IPage<PatientQueueVO>> getPatientQueue(PatientQueueDTO dto);

    /** 获取患者药品项目（处方+药品信息分页） */
    JsonVO<IPage<PatientDrugItemVO>> getPatientDrugItem(PatientDrugItemDTO dto);
}
