package com.zeroone.star.project.j3.patient;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.patient.PatientCreateDTO;
import com.zeroone.star.project.dto.j3.patient.PatientUpdateDTO;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.patient.PatientListVO;
import com.zeroone.star.project.vo.j3.patient.PatientOptionVO;
import com.zeroone.star.project.vo.j3.patient.PatientVO;

/**
 * <p>
 * 描述：患者管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 波小夏
 * @author 照夜清
 * @version 1.0.0
 */
public interface PatientApis {

    /**
     * 获取选择患者列表。
     *
     * @param query 查询条件
     * @return 患者选择分页列表
     */
    JsonVO<PageDTO<PatientOptionVO>> queryPatients(PatientQuery query);

    /**
     * 获取患者管理列表。
     *
     * @param query 查询条件
     * @return 患者管理分页列表
     */
    JsonVO<PageDTO<PatientListVO>> queryPatientPage(PatientQuery query);

    /**
     * 获取患者详情。
     *
     * @param id 患者ID
     * @return 患者详情
     */
    JsonVO<PatientVO> getPatient(String id);

    /**
     * 新增患者。
     *
     * @param dto 新增患者数据
     * @return 新增结果
     */
    JsonVO<String> createPatient(PatientCreateDTO dto);

    /**
     * 修改患者。
     *
     * @param id 患者ID
     * @param dto 修改患者数据
     * @return 修改结果
     */
    JsonVO<String> updatePatient(String id, PatientUpdateDTO dto);

    /**
     * 删除患者，采用软删除。
     *
     * @param id 患者ID
     * @return 删除结果
     */
    JsonVO<String> deletePatient(String id);
}