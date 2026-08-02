package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.DiagnosisCatalogConstants;
import com.zeroone.star.basedata.entity.diagnosis.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.mapper.DiagnosisMapper;
import com.zeroone.star.basedata.service.IDiagnosisService;
import com.zeroone.star.project.components.user.UserHolder;

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
import org.springframework.util.StringUtils;

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

    private static final long DEFAULT_PAGE_INDEX = 1L;
    private static final long DEFAULT_PAGE_SIZE = 10L;

    @Autowired
    private MsDiagnosisMapper msDiagnosisMapper;

    @Autowired
    private UserHolder userHolder;

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
        // 1. 处理分页参数、目录编码和关键字，Service 内部调用也使用默认分页值
        long pageIndex = normalizePageIndex(query == null ? 0L : query.getPageIndex());
        long pageSize = normalizePageSize(query == null ? 0L : query.getPageSize());
        String keyword = query == null ? null : StrUtil.trim(query.getKeyword());
        String sourceEnum = query == null ? null : StrUtil.trim(query.getSourceEnum());

        // 2. 从公共编码接口接入前的临时常量中过滤目录，并转换为页面显示对象
        List<DiagnosisOptionVO> matched = DiagnosisCatalogConstants.catalogOptions().entrySet().stream()
                .filter(item -> StrUtil.isBlank(sourceEnum) || item.getKey().equals(sourceEnum))
                .filter(item -> StrUtil.isBlank(keyword)
                        || StrUtil.containsIgnoreCase(item.getKey(), keyword)
                        || StrUtil.containsIgnoreCase(item.getValue(), keyword))
                .map(item -> {
                    DiagnosisOptionVO option = new DiagnosisOptionVO();
                    option.setCode(item.getKey());
                    option.setDisplay(item.getValue());
                    return option;
                })
                .toList();

        // 3. 对过滤后的目录数据进行内存分页，越界页返回空 rows 但保留总数
        long total = matched.size();
        long pages = total / pageSize + (total % pageSize == 0 ? 0 : 1);
        long fromIndex = pageIndex > pages ? total : (pageIndex - 1) * pageSize;
        long toIndex = pageSize >= total - fromIndex ? total : fromIndex + pageSize;

        // 4. 封装为统一的 PageDTO 返回，保持前端接口协议不变
        Page<DiagnosisOptionVO> result = new Page<>(pageIndex, pageSize, total);
        result.setRecords(matched.subList((int) fromIndex, (int) toIndex));
        return PageDTO.create(result);
    }

    @Override
    public PageDTO<DiagnosisListVO> queryDiagnosisList(DiagnosisPageQuery query) {
        // 1. 处理分页参数，避免 Service 被内部调用时传入 0 导致分页异常
        long pageIndex = normalizePageIndex(query == null ? 0L : query.getPageIndex());
        long pageSize = normalizePageSize(query == null ? 0L : query.getPageSize());
        Page<ClnConditionDefinitionDO> page = new Page<>(pageIndex, pageSize);

        // 2. 按关键字、诊断目录和当前租户构造组合查询条件
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = buildQueryWrapper(
                query == null ? null : query.getKeyword(), query == null ? null : query.getSourceEnum());
        wrapper.eq(ClnConditionDefinitionDO::getTenantId, currentTenantId());
        wrapper.orderByAsc(ClnConditionDefinitionDO::getConditionCode)
                .orderByAsc(ClnConditionDefinitionDO::getId);

        // 3. 按诊断编码和主键稳定排序后查询诊断定义表
        Page<ClnConditionDefinitionDO> result = baseMapper.selectPage(page, wrapper);

        // 4. 将数据库实体转换为列表 VO，避免向前端暴露系统字段
        return PageDTO.create(result, msDiagnosisMapper::entityToListVo);
    }

    @Override
    public DiagnosisDetailVO queryDiagnosisDetail(String id) {
        // 1. 校验诊断 ID，防止绕过 Controller 时发起无效查询
        if (StrUtil.isBlank(id)) {
            throw new IllegalArgumentException("诊断ID不能为空");
        }
        ClnConditionDefinitionDO entity = lambdaQuery()
                .eq(ClnConditionDefinitionDO::getId, id)
                .eq(ClnConditionDefinitionDO::getTenantId, currentTenantId())
                .one();
        if (entity == null) {
            throw new RuntimeException("诊断记录不存在");
        }

        // 2. 按诊断 ID 和当前租户查询，避免跨租户读取
        return msDiagnosisMapper.entityToDiagnosisDetailVo(entity);
    }

    private LambdaQueryWrapper<ClnConditionDefinitionDO> buildQueryWrapper(
            String keyword, String sourceEnum) {
        // 关键字条件使用括号分组，确保 OR 只作用于名称、编码、拼音和五笔字段
        String normalizedKeyword = StrUtil.trim(keyword);
        String normalizedSourceEnum = StrUtil.trim(sourceEnum);
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        if (StrUtil.isNotBlank(normalizedKeyword)) {
            wrapper.and(item -> item
                    .like(ClnConditionDefinitionDO::getName, normalizedKeyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getConditionCode, normalizedKeyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getPyStr, normalizedKeyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getWbStr, normalizedKeyword));
        }
        if (StrUtil.isNotBlank(normalizedSourceEnum)) {
            wrapper.eq(ClnConditionDefinitionDO::getSourceEnum, normalizedSourceEnum);
        }
        return wrapper;
    }

    private long normalizePageIndex(long pageIndex) {
        return pageIndex > 0 ? pageIndex : DEFAULT_PAGE_INDEX;
    }

    private long normalizePageSize(long pageSize) {
        return pageSize > 0 ? pageSize : DEFAULT_PAGE_SIZE;
    }

    private String currentTenantId() {
        // 统一从用户上下文获取租户，不使用固定租户或空租户降级
        try {
            String tenantId = userHolder.getCurrentTenantId();
            if (!StringUtils.hasText(tenantId)) {
                throw new IllegalStateException("当前用户未绑定租户");
            }
            return tenantId;
        } catch (Exception e) {
            if (e instanceof IllegalStateException) {
                throw (IllegalStateException) e;
            }
            throw new IllegalStateException("获取当前租户失败", e);
        }
    }



}
