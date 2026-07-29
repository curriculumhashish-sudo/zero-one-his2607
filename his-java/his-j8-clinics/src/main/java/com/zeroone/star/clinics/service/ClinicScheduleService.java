// service/IClinicScheduleService.java
package com.zeroone.star.clinics.service;

import com.zeroone.star.project.dto.j8.clinics.AllDayStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.SubstituteDTO;

/**
 * 门诊排班管理业务服务接口
 */
public interface ClinicScheduleService {

    /**
     * 设置替诊
     */
    void setSubstitute(SubstituteDTO dto);

    /**
     * 撤销替诊
     */
    void cancelSubstitute(Long scheduleId);

    /**
     * 整日停诊
     */
    void executeAllDayStopClinic(AllDayStopClinicExecuteDTO dto);

    /**
     * 部分停诊
     */
    void executePartialStopClinic(PartialStopClinicExecuteDTO dto);

    /**
     * 解锁停诊
     */
    void unlockStopClinic(Long scheduleId);
}