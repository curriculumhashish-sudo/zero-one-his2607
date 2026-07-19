package com.zeroone.star.basedata.mapper;

import com.zeroone.star.project.dto.j4.diagnosis.MedicalInsuranceMatchDTO;
import com.zeroone.star.project.vo.j4.diagnosis.MedicalInsuranceMatchVO;
import org.mapstruct.Mapper;
import org.mapstruct.ReportingPolicy;

/**
 * 诊断管理 MapStruct 映射接口。
 *
 * @author j4组
 * @version 1.0.0
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsDiagnosisMapper {

    /**
     * 将医保对照请求数据转换为显示对象。
     *
     * @param dto 医保对照请求数据
     * @return 医保对照显示对象
     */
    MedicalInsuranceMatchVO medicalInsuranceMatchDtoToVo(MedicalInsuranceMatchDTO dto);
}
