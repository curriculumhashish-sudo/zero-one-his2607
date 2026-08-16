package com.zeroone.star.report.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.report.entity.AdmEncounterDiagnosis;

import java.util.List;
import java.util.Map;

/**
 * 就诊诊断 Mapper 接口
 * @author j6小组-TKORHP
 */
public interface AdmEncounterDiagnosisMapper extends BaseMapper<AdmEncounterDiagnosis> {

    /**
     * 统计患者疾病类型分布
     * <p>关联链路：adm_encounter → adm_encounter_diagnosis → cln_condition → cln_condition_definition</p>
     * @return 疾病分布列表，每项包含 diseaseName(疾病名称) 和 count(就诊人数)
     */
    List<Map<String, Object>> selectDiseaseDistribution();
}
