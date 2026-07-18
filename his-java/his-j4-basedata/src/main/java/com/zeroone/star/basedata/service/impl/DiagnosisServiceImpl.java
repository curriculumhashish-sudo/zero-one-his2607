package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.Diagnosis;
import com.zeroone.star.basedata.mapper.DiagnosisMapper;
import com.zeroone.star.basedata.service.IDiagnosisService;
import com.zeroone.star.project.dto.j4.diagnosis.CreateDiagnosisDTO;
import com.zeroone.star.project.dto.j4.diagnosis.UpdateDiagnosisDTO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisVO;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

@Service
public class DiagnosisServiceImpl extends ServiceImpl<DiagnosisMapper, Diagnosis> 
        implements IDiagnosisService {

    private static final String VALID_FLAG_ENABLE = "1";

    @Override
    @Transactional
    public DiagnosisVO createDiagnosis(CreateDiagnosisDTO request) {
        Diagnosis diagnosis = new Diagnosis();
        diagnosis.setCaseRecordId(request.getCaseRecordId());
        diagnosis.setTenantId("default");
        diagnosis.setDiagTypeCode(request.getDiagTypeCode());
        diagnosis.setSortNo(request.getSortNo() != null ? request.getSortNo() : 0);
        diagnosis.setDiagCode(request.getDiagCode());
        diagnosis.setDiagName(request.getDiagName());
        diagnosis.setAdmCondCode(request.getAdmCondCode());
        diagnosis.setMaindiagFlag(request.getMaindiagFlag());
        diagnosis.setValiFlag(StringUtils.hasText(request.getValiFlag()) ? request.getValiFlag() : VALID_FLAG_ENABLE);

        save(diagnosis);
        return convertToVO(diagnosis);
    }

    @Override
    @Transactional
    public DiagnosisVO updateDiagnosis(String id, UpdateDiagnosisDTO request) {
        Diagnosis diagnosis = getById(id);
        if (diagnosis == null) {
            throw new RuntimeException("诊断记录不存在");
        }

        if (StringUtils.hasText(request.getCaseRecordId())) {
            diagnosis.setCaseRecordId(request.getCaseRecordId());
        }
        if (StringUtils.hasText(request.getDiagTypeCode())) {
            diagnosis.setDiagTypeCode(request.getDiagTypeCode());
        }
        if (request.getSortNo() != null) {
            diagnosis.setSortNo(request.getSortNo());
        }
        if (StringUtils.hasText(request.getDiagCode())) {
            diagnosis.setDiagCode(request.getDiagCode());
        }
        if (StringUtils.hasText(request.getDiagName())) {
            diagnosis.setDiagName(request.getDiagName());
        }
        if (StringUtils.hasText(request.getAdmCondCode())) {
            diagnosis.setAdmCondCode(request.getAdmCondCode());
        }
        if (StringUtils.hasText(request.getMaindiagFlag())) {
            diagnosis.setMaindiagFlag(request.getMaindiagFlag());
        }
        if (StringUtils.hasText(request.getValiFlag())) {
            diagnosis.setValiFlag(request.getValiFlag());
        }

        updateById(diagnosis);
        return convertToVO(diagnosis);
    }

    @Override
    @Transactional
    public void deleteDiagnosis(String id) {
        Diagnosis diagnosis = getById(id);
        if (diagnosis == null) {
            throw new RuntimeException("诊断记录不存在");
        }
        removeById(id);
    }

    @Override
    @Transactional
    public DiagnosisVO toggleDiagnosisStatus(String id, String valiFlag) {
        Diagnosis diagnosis = getById(id);
        if (diagnosis == null) {
            throw new RuntimeException("诊断记录不存在");
        }

        diagnosis.setValiFlag(valiFlag);
        updateById(diagnosis);
        return convertToVO(diagnosis);
    }

    private DiagnosisVO convertToVO(Diagnosis diagnosis) {
        DiagnosisVO vo = new DiagnosisVO();
        vo.setId(diagnosis.getId());
        vo.setCaseRecordId(diagnosis.getCaseRecordId());
        vo.setTenantId(diagnosis.getTenantId());
        vo.setDiagTypeCode(diagnosis.getDiagTypeCode());
        vo.setSortNo(diagnosis.getSortNo());
        vo.setDiagCode(diagnosis.getDiagCode());
        vo.setDiagName(diagnosis.getDiagName());
        vo.setAdmCondCode(diagnosis.getAdmCondCode());
        vo.setMaindiagFlag(diagnosis.getMaindiagFlag());
        vo.setValiFlag(diagnosis.getValiFlag());
        vo.setCreatedAt(diagnosis.getCreatedAt());
        vo.setUpdatedAt(diagnosis.getUpdatedAt());
        return vo;
    }
}