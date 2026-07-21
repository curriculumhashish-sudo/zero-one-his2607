package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.mapper.DiagnosisMapper;
import com.zeroone.star.basedata.service.IDiagnosisService;

import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;

import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：诊断服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class DiagnosisServiceImpl extends ServiceImpl<DiagnosisMapper, ClnConditionDefinitionDO> 
    implements IDiagnosisService {

    @Autowired
    private MsDiagnosisMapper msDiagnosisMapper;

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
          
        return null;
    }

    @Override
    @Transactional
    public String saveDefinition(DiagnosisDefinitionDTO dto) {
          
        return null;
    }

    @Override
    @Transactional
    public void modifyDefinition(DiagnosisDefinitionDTO dto) {
        
    }

    @Override
    @Transactional
    public void batchModifyStatus(List<String> ids, String status) {
        
    }

    @Override
    @Transactional
    public void batchDelete(List<String> ids) {
        
    }

    @Override
    public Page<ClnConditionDefinitionDO> getSelectList(DiagnosisDefinitionQuery query) {
        
        return null;
    }


}