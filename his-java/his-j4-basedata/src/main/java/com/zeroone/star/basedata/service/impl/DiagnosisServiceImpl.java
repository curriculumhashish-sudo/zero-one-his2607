package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.DiagnosisCatalogConstants;
import com.zeroone.star.basedata.entity.diagnosis.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.mapper.DiagnosisMapper;
import com.zeroone.star.basedata.service.IDiagnosisService;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisSaveDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisUpdateDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisIdsDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisStatusDTO;

import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisPageQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisSelectPageQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

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
    public String saveDefinition(DiagnosisSaveDTO dto) {
          
        return null;
    }

    @Override
    @Transactional
    public void modifyDefinition(DiagnosisUpdateDTO dto) {
        
    }

    @Override
    @Transactional
    public void modifyBatchStatus(DiagnosisStatusDTO dto) {
        
    }

    @Override
    @Transactional
    public void removeBatch(DiagnosisIdsDTO dto) {
        
    }

    @Override
    public Page<ClnConditionDefinitionDO> getSelectList(DiagnosisDefinitionQuery query) {
        
        return null;
    }

    @Override
    public PageDTO<DiagnosisOptionVO> queryDiagnosisSelectList(DiagnosisSelectPageQuery query) {
        long pageIndex = query.getPageIndex() <= 0 ? 1L : query.getPageIndex();
        long pageSize = query.getPageSize() <= 0 ? 10L : query.getPageSize();
        String keyword = StrUtil.trim(query.getKeyword());
        String sourceEnum = StrUtil.trim(query.getSourceEnum());

        List<DiagnosisOptionVO> matched = DiagnosisCatalogConstants.catalogOptions().entrySet().stream()
                .filter(item -> StrUtil.isBlank(sourceEnum) || item.getKey().equals(sourceEnum))
                .filter(item -> StrUtil.isBlank(keyword)
                        || item.getKey().contains(keyword)
                        || item.getValue().contains(keyword))
                .map(item -> {
                    DiagnosisOptionVO option = new DiagnosisOptionVO();
                    option.setCode(item.getKey());
                    option.setDisplay(item.getValue());
                    return option;
                })
                .toList();

        long total = matched.size();
        long fromIndex = Math.min((pageIndex - 1) * pageSize, total);
        long toIndex = Math.min(fromIndex + pageSize, total);

        Page<DiagnosisOptionVO> result = new Page<>(pageIndex, pageSize, total);
        result.setRecords(matched.subList((int) fromIndex, (int) toIndex));
        return PageDTO.create(result);
    }

    @Override
    public PageDTO<DiagnosisListVO> queryDiagnosisList(DiagnosisPageQuery query) {
        Page<ClnConditionDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = buildQueryWrapper(
                query.getKeyword(), query.getSourceEnum());
        wrapper.orderByAsc(ClnConditionDefinitionDO::getConditionCode);

        Page<ClnConditionDefinitionDO> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msDiagnosisMapper::entityToListVo);
    }

    @Override
    public DiagnosisDetailVO queryDiagnosisDetail(String id) {
        ClnConditionDefinitionDO entity = baseMapper.selectById(id);
        return entity == null ? null : msDiagnosisMapper.entityToDiagnosisDetailVo(entity);
    }

    private LambdaQueryWrapper<ClnConditionDefinitionDO> buildQueryWrapper(
            String keyword, String sourceEnum) {
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        if (StrUtil.isNotBlank(keyword)) {
            wrapper.and(item -> item
                    .like(ClnConditionDefinitionDO::getName, keyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getConditionCode, keyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getPyStr, keyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getWbStr, keyword));
        }
        if (StrUtil.isNotBlank(sourceEnum)) {
            wrapper.eq(ClnConditionDefinitionDO::getSourceEnum, sourceEnum);
        }
        return wrapper;
    }



}
