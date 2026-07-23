package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.entity.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.service.IClnConditionDefinitionService;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.dto.j4.diagnosis.YbMappingDTO;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DisableCheckResultVO;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * @Description:
 * @Created: with IntelliJ IDEA.
 * @author: yc
 * @createTime: 2026-07-23 19:34
 **/
@Service
public class ClnConditionDefinitionServiceImpl implements IClnConditionDefinitionService {
    @Override
    public Page<ClnConditionDefinitionDO> pageQuery(DiagnosisDefinitionQuery query) {
        return null;
    }

    @Override
    public ClnConditionDefinitionDO getById(String id) {
        return null;
    }

    @Override
    public List<DiagnosisDefinitionDetailVO.ReferenceDetail> getReferences(String id) {
        return List.of();
    }

    @Override
    public String saveDefinition(DiagnosisDefinitionDTO dto) {
        return "";
    }

    @Override
    public void modifyDefinition(DiagnosisDefinitionDTO dto) {

    }

    @Override
    public DisableCheckResultVO queryDisableCheck(String id) {
        return null;
    }

    @Override
    public void modifyDisableDefinition(String id) {

    }

    @Override
    public void updateYbMapping(YbMappingDTO dto) {

    }
}
