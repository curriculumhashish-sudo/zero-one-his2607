package com.zeroone.star.project.j3.patient;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.patient.PatientOptionVO;
import com.zeroone.star.project.vo.j3.patient.PatientVO;

/**
 * <p>
 * 描述：患者管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 波小夏
 * @version 1.0.0
 */
public interface PatientApis {

    /**
     * 获取选择患者列表
     * @param query 查询条件
     * @return 患者分页列表
     */
    JsonVO<PageDTO<PatientOptionVO>> queryPatients(PatientQuery query);

    /**
     * 获取患者详情
     * @param id 患者ID
     * @return 患者详情
     */
    JsonVO<PatientVO> getPatient(String id);

    /**
     * 删除患者（软删除）
     * @param id 患者ID
     * @return 删除结果
     */
    JsonVO<String> deletePatient(String id);
}
