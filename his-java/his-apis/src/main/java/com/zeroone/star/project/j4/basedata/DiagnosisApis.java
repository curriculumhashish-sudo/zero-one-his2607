package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisStatusDTO;
import com.zeroone.star.project.dto.j4.diagnosis.MedicalInsuranceMatchDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.MedicalInsuranceMatchVO;

/**
 * <p>
 * 描述：诊断管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface DiagnosisApis {

    /**
     * 批量启用诊断。
     *
     * @param dto 诊断主键集合
     * @return 操作结果
     */
    JsonVO<Void> modifyDiagnosisEnable(DiagnosisStatusDTO dto);

    /**
     * 批量停用诊断。
     *
     * @param dto 诊断主键集合
     * @return 操作结果
     */
    JsonVO<Void> modifyDiagnosisDisable(DiagnosisStatusDTO dto);

    /**
     * 更新诊断医保对照信息。
     *
     * @param dto 医保对照信息
     * @return 更新后的医保对照信息
     */
    JsonVO<MedicalInsuranceMatchVO> updateMedicalInsurance(MedicalInsuranceMatchDTO dto);
}
