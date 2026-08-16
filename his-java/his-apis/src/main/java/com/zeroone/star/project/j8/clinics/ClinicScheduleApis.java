package com.zeroone.star.project.j8.clinics;


import com.zeroone.star.project.dto.j8.clinics.AllDayStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.PartialStopClinicExecuteDTO;
import com.zeroone.star.project.dto.j8.clinics.SubstituteDTO;
import com.zeroone.star.project.dto.j8.clinics.UnlockStopClinicDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.clinics.StopClinicResultVO;
import com.zeroone.star.project.vo.j8.clinics.SubstituteVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

@Tag(name = "门诊排班管理")
public interface ClinicScheduleApis {

    @Operation(summary = "设置替诊")
    @PostMapping("/setroster/set-substitute")
    JsonVO<SubstituteVO> setSubstitute(@RequestBody SubstituteDTO dto);

    @Operation(summary = "撤销替诊")
    @PostMapping("/setroster/cancel-substitute")
    JsonVO<SubstituteVO> cancelSubstitute(@RequestParam String scheduleId);
    

    @Operation(summary = "整日停诊")
    @PostMapping("/setroster/execute-stop-all-day")
    JsonVO<StopClinicResultVO> executeAllDayStopClinic(@RequestBody AllDayStopClinicExecuteDTO dto);

    @Operation(summary = "部分停诊")
    @PostMapping("/setroster/execute-stop-partial")
    JsonVO<StopClinicResultVO> executePartialStopClinic(@RequestBody PartialStopClinicExecuteDTO dto);

    @Operation(summary = "解锁停诊")
    @PostMapping("/setroster/unlock-stop-clinic")
    JsonVO<String> unlockStopClinic(@RequestBody UnlockStopClinicDTO dto);
}