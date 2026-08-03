package com.zeroone.star.project.j8.clinics;


//import com.zeroone.star.project.dto.j8.clinics.RetireScheduleDTO;
import com.zeroone.star.project.dto.j8.clinics.RetireScheduleDTO;
import com.zeroone.star.project.dto.j8.clinics.SlotBatchReleaseDTO;
import com.zeroone.star.project.query.j8.clinics.RelSlotQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.clinics.ResumeScheduleVO;
import com.zeroone.star.project.vo.j8.clinics.SlotBatchReleaseVO;
import com.zeroone.star.project.vo.j8.clinics.SlotVO;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;

import java.util.List;

/**
 * <p>
 * 描述：门诊排班管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
public interface ScheduleApis {

    /**
     * 获取已放号源列表
     * @param query 查询参数
     * @return 响应对象
     */
    JsonVO<List<SlotVO>> getRelNumberSource(@Valid RelSlotQuery query);


    /**
     * 删除号源
     * @param slotId 待删除的号源时段主键id
     * @return 响应对象
     */
    JsonVO<String> deleteNumberSource(@Schema(description = "号源时段主键id", example = "335659579333414912")String slotId);

    /**
     * 批量放号
     * @param numberSourceBatchReleaseDTO 批量待放号参数
     * @return 响应对象
     */
    JsonVO<SlotBatchReleaseVO> batchReleaseNumberSource(@Valid SlotBatchReleaseDTO numberSourceBatchReleaseDTO);


    /**
     * 恢复开诊
     * @param scheduleId 排班id
     * @return 响应对象
     */
    JsonVO<ResumeScheduleVO> reactivateSchedule(String scheduleId);

    /**
     * 作废日程
     * @param retireScheduleDTO
     * @return 响应对象
     */
    JsonVO<String> retireSchedule(@Valid RetireScheduleDTO retireScheduleDTO);
}
