package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.configuration.entity.EmrTemplateDO;
import com.zeroone.star.configuration.mapper.EmrTemplateMapper;
import com.zeroone.star.configuration.service.IEmrTemplateService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j5.emr.MedicalTemplateQuery;
import com.zeroone.star.project.vo.j5.emr.MedicalTemplateDetailVO;
import com.zeroone.star.project.vo.j5.emr.MedicalTemplateVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

/**
 * 病历模板服务实现类
 */
@Service
public class EmrTemplateServiceImpl extends ServiceImpl<EmrTemplateMapper, EmrTemplateDO> implements IEmrTemplateService {

    @Resource
    private MsEmrTemplateMapper msEmrTemplateMapper;

    /**
     * 分页查询病历模板列表
     * 仅查询列表展示所需字段，避免 SELECT *
     */
    @Override
    public PageDTO<MedicalTemplateVO> listTemplates(MedicalTemplateQuery query) {
        Page<EmrTemplateDO> page = new Page<>(query.getPageIndex(), query.getPageSize());

        QueryWrapper<EmrTemplateDO> queryWrapper = new QueryWrapper<>();
        // 仅查询列表展示所需字段
        queryWrapper.select("id", "name", "document_type_code", "content",
                "is_enabled", "scope_level", "created_at", "updated_at",
                "created_by", "updated_by");

        // 条件筛选：模板名称模糊查询
        if (StringUtils.hasText(query.getTemplateName())) {
            queryWrapper.like("name", query.getTemplateName());
        }
        // 条件筛选：文书类型编码精确匹配
        if (StringUtils.hasText(query.getDocumentTypeCode())) {
            queryWrapper.eq("document_type_code", query.getDocumentTypeCode());
        }
        // 条件筛选：启用状态
        if (query.getIsEnabled() != null) {
            queryWrapper.eq("is_enabled", query.getIsEnabled());
        }

        queryWrapper.orderByDesc("created_at");

        Page<EmrTemplateDO> resultPage = baseMapper.selectPage(page, queryWrapper);
        return PageDTO.create(resultPage, msEmrTemplateMapper::toListVO);
    }

    /**
     * 根据ID获取病历模板详情
     */
    @Override
    public MedicalTemplateDetailVO getTemplateDetail(String id) {
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("模板ID不能为空");
        }

        QueryWrapper<EmrTemplateDO> queryWrapper = new QueryWrapper<>();
        // 查询详情所需全部字段，不使用 SELECT *
        queryWrapper.select("id", "name", "code", "document_type_code",
                "encounter_class_enum", "encounter_type_enum", "content",
                "is_enabled", "scope_level", "organization_id",
                "created_at", "updated_at", "created_by", "updated_by");
        queryWrapper.eq("id", id);

        EmrTemplateDO entity = baseMapper.selectOne(queryWrapper);
        if (entity == null) {
            throw new RuntimeException("病历模板不存在");
        }

        return msEmrTemplateMapper.toDetailVO(entity);
    }
}
