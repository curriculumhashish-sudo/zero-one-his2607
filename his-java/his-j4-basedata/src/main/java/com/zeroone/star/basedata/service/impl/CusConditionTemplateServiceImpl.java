package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.service.ICusConditionTemplateService;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisTemplateDTO;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisTemplateQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisTemplateVO;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * @Description:
 * @Created: with IntelliJ IDEA.
 * @author: yc
 * @createTime: 2026-07-23 19:35
 **/
@Service
public class CusConditionTemplateServiceImpl implements ICusConditionTemplateService {
    @Override
    public List<DiagnosisTemplateVO> pageQuery(DiagnosisTemplateQuery query) {
        return List.of();
    }

    @Override
    public String saveTemplate(DiagnosisTemplateDTO dto) {
        return "";
    }

    @Override
    public void modifyTemplate(DiagnosisTemplateDTO dto) {

    }

    @Override
    public void deleteTemplate(String id) {

    }
}
