package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.configuration.entity.EmrDocumentTypeDO;
import com.zeroone.star.configuration.mapper.EmrDocumentTypeMapper;
import com.zeroone.star.configuration.service.IEmrDocumentTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.exception.BusinessException;
import com.zeroone.star.project.query.j5.configuration_management.medical_record_template.EmrDocumentTypeQuery;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeDetailVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeNameVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeVO;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：获取文书类型serviceImpl
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author yjyl
 * @version 1.0.0
 */
@Service
public class EmrDocumentTypeServiceImpl
        extends ServiceImpl<EmrDocumentTypeMapper, EmrDocumentTypeDO>
        implements IEmrDocumentTypeService {

    /**
     * 获取文书类型下拉字典（仅启用）。
     * @return 文书类型下拉列表；无数据时返回空列表
     */
    @Override
    public List<EmrDocumentTypeNameVO> getEmrDocumentTypeName() {
        LambdaQueryWrapper<EmrDocumentTypeDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(EmrDocumentTypeDO::getCode,
                       EmrDocumentTypeDO::getName,
                       EmrDocumentTypeDO::getSortOrder)
               .eq(EmrDocumentTypeDO::getIsEnabled, 1)
               .orderByAsc(EmrDocumentTypeDO::getSortOrder);

        List<EmrDocumentTypeDO> entities = baseMapper.selectList(wrapper);

        List<EmrDocumentTypeNameVO> rows = new ArrayList<>(entities.size());
        for (EmrDocumentTypeDO e : entities) {
            EmrDocumentTypeNameVO vo = new EmrDocumentTypeNameVO();
            vo.setCode(e.getCode());
            vo.setName(e.getName());
            rows.add(vo);
        }
        return rows;
    }

    /**
     * 分页查询文书类型列表（含启用与禁用，用于后台管理）。
     */
    @Override
    public PageDTO<EmrDocumentTypeVO> queryEmrDocumentType(EmrDocumentTypeQuery query) {
        if (query == null) {
            throw new BusinessException("查询条件不能为空");
        }

        // pageSize 上限保护，防止恶意请求拖垮数据库
        if (query.getPageSize() > 100) {
            query.setPageSize(100L);
        }

        Page<EmrDocumentTypeDO> page = new Page<>(query.getPageIndex(), query.getPageSize());

        LambdaQueryWrapper<EmrDocumentTypeDO> wrapper = new LambdaQueryWrapper<>();
        if (StringUtils.hasText(query.getKeyword())) {
            // 转义 SQL LIKE 通配符，避免用户输入的 % _ \ 被当作通配符
            String keyword = query.getKeyword().trim()
                    .replace("\\", "\\\\")
                    .replace("%", "\\%")
                    .replace("_", "\\_");
            wrapper.and(w -> w.like(EmrDocumentTypeDO::getCode, keyword)
                               .or().like(EmrDocumentTypeDO::getName, keyword));
        }
        wrapper.orderByAsc(EmrDocumentTypeDO::getSortOrder)
               .orderByAsc(EmrDocumentTypeDO::getId);

        Page<EmrDocumentTypeDO> entityPage = baseMapper.selectPage(page, wrapper);

        List<EmrDocumentTypeVO> rows = new ArrayList<>(entityPage.getRecords().size());
        for (EmrDocumentTypeDO e : entityPage.getRecords()) {
            rows.add(toEmrDocumentTypeVO(e));
        }

        PageDTO<EmrDocumentTypeVO> result = new PageDTO<>();
        result.setPageIndex(entityPage.getCurrent());
        result.setPageSize(entityPage.getSize());
        result.setTotal(entityPage.getTotal());
        result.setPages(entityPage.getPages());
        result.setRows(rows);
        return result;
    }

    /**
     * 根据主键查询文书类型详情。
     */
    @Override
    public EmrDocumentTypeDetailVO getEmrDocumentTypeDetail(String id) {
        if (!StringUtils.hasText(id)) {
            throw new BusinessException("文书类型主键ID不能为空");
        }
        EmrDocumentTypeDO entity = baseMapper.selectById(id);
        if (entity == null) {
            throw new BusinessException("文书类型不存在或已被删除");
        }
        return toEmrDocumentTypeDetailVO(entity);
    }

    /**
     * DO → EmrDocumentTypeVO（列表 VO）。
     */
    private EmrDocumentTypeVO toEmrDocumentTypeVO(EmrDocumentTypeDO e) {
        EmrDocumentTypeVO vo = new EmrDocumentTypeVO();
        vo.setId(e.getId());
        vo.setCode(e.getCode());
        vo.setName(e.getName());
        vo.setIsEnabled(e.getIsEnabled());
        return vo;
    }

    /**
     * DO → EmrDocumentTypeDetailVO（详情 VO）。
     */
    private EmrDocumentTypeDetailVO toEmrDocumentTypeDetailVO(EmrDocumentTypeDO e) {
        EmrDocumentTypeDetailVO vo = new EmrDocumentTypeDetailVO();
        vo.setId(e.getId());
        vo.setCode(e.getCode());
        vo.setName(e.getName());
        vo.setDefaultTemplateId(e.getDefaultTemplateId());
        vo.setIsEnabled(e.getIsEnabled());
        vo.setSortOrder(e.getSortOrder());
        return vo;
    }
}
