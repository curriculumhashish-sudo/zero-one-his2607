// service/IClinicScheduleService.java
package com.zeroone.star.clinics.service;

import com.zeroone.star.project.dto.j8.clinics.AllDayStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.SubstituteDTO;
import com.zeroone.star.project.dto.j8.clinics.UnlockStopClinicDTO;
import com.zeroone.star.project.vo.j8.clinics.StopClinicResultVO;
import com.zeroone.star.project.vo.j8.clinics.SubstituteVO;

/**
 * 门诊排班管理业务服务接口
 */
public interface ClinicScheduleService {

    /**
     * 设置替诊
     */
    SubstituteVO setSubstitute(SubstituteDTO dto);

    /**
     * 撤销替诊
     */
    SubstituteVO cancelSubstitute(String scheduleId);

    /**
     * 整日停诊
     */
    StopClinicResultVO executeAllDayStopClinic(AllDayStopClinicExecuteDTO dto);

    /**
     * 部分停诊
     */
    StopClinicResultVO executePartialStopClinic(PartialStopClinicExecuteDTO dto);

    /**
     * 解锁停诊
     */
    void unlockStopClinic(UnlockStopClinicDTO dto);
}