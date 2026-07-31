package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.configuration.entity.TreatmentBinding;
import com.zeroone.star.configuration.entity.UsageBinding;
import com.zeroone.star.configuration.mapper.TreatmentBindingMapper;
import com.zeroone.star.configuration.mapper.UsageBindingMapper;
import com.zeroone.star.configuration.service.BindingMappingService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.BindingInitDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.TreatmentBindingDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.UsageBindingDTO;
import com.zeroone.star.project.exception.BusinessException;
import com.zeroone.star.project.query.j5.bindingMapping.TreatmentBindingQuery;
import com.zeroone.star.project.query.j5.bindingMapping.UsageBindingQuery;
import com.zeroone.star.project.vo.JsonVO;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * 诊疗/用法绑定映射服务实现
 */
@Service
@RequiredArgsConstructor
public class BindingMappingServiceImpl implements BindingMappingService {

    private final TreatmentBindingMapper treatmentMapper;
    private final UsageBindingMapper usageMapper;
    private final UserHolder userHolder;

    /**
     * 返回绑定映射初始化下拉选项
     */
    public BindingInitDTO initBindingOptions() {
        BindingInitDTO dto = new BindingInitDTO();

        // 诊疗类型下拉
        List<BindingInitDTO.OptionItem> treatmentOptions = new ArrayList<>();
        dto.setTreatmentOptions(treatmentOptions);

        // 用法类型下拉
        List<BindingInitDTO.OptionItem> usageOptions = new ArrayList<>();
        dto.setUsageOptions(usageOptions);

        // 科室下拉
        List<BindingInitDTO.OptionItem> orgOptions = new ArrayList<>();
        dto.setOrgOptions(orgOptions);

        // 物品类型下拉
        List<BindingInitDTO.OptionItem> itemTypeOptions = new ArrayList<>();
        dto.setItemTypeOptions(itemTypeOptions);

        return dto;
    }

    @Override
    public JsonVO<PageDTO<TreatmentBindingDTO>> queryAllTreatmentBindings(TreatmentBindingQuery query) {
        Page<TreatmentBinding> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<TreatmentBinding> wrapper = new LambdaQueryWrapper<>();
        if (query.getTreatmentId() != null) {
            wrapper.eq(TreatmentBinding::getTreatmentId, query.getTreatmentId());
        }
        wrapper.orderByDesc(TreatmentBinding::getUpdatedAt);

        Page<TreatmentBinding> result = treatmentMapper.selectPage(page, wrapper);
        List<TreatmentBindingDTO> dtoList = new ArrayList<>();
        for (TreatmentBinding record : result.getRecords()) {
            TreatmentBindingDTO dto = new TreatmentBindingDTO();
            BeanUtils.copyProperties(record, dto);
            dtoList.add(dto);
        }

        PageDTO<TreatmentBindingDTO> pageDTO = new PageDTO<>();
        pageDTO.setRows(dtoList);
        pageDTO.setTotal(result.getTotal());
        pageDTO.setPageIndex(result.getCurrent());
        pageDTO.setPageSize(result.getSize());
        pageDTO.setPages(result.getPages());
        return JsonVO.success(pageDTO);
    }

    @Override
    public JsonVO<PageDTO<UsageBindingDTO>> queryAllUsageBindings(UsageBindingQuery query) {
        Page<UsageBinding> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<UsageBinding> wrapper = new LambdaQueryWrapper<>();
        if (query.getUsageId() != null) {
            wrapper.eq(UsageBinding::getUsageId, query.getUsageId());
        }
        wrapper.orderByDesc(UsageBinding::getUpdatedAt);

        Page<UsageBinding> result = usageMapper.selectPage(page, wrapper);
        List<UsageBindingDTO> dtoList = new ArrayList<>();
        for (UsageBinding record : result.getRecords()) {
            UsageBindingDTO dto = new UsageBindingDTO();
            BeanUtils.copyProperties(record, dto);
            dtoList.add(dto);
        }

        PageDTO<UsageBindingDTO> pageDTO = new PageDTO<>();
        pageDTO.setRows(dtoList);
        pageDTO.setTotal(result.getTotal());
        pageDTO.setPageIndex(result.getCurrent());
        pageDTO.setPageSize(result.getSize());
        pageDTO.setPages(result.getPages());
        return JsonVO.success(pageDTO);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> addTreatmentBinding(TreatmentBindingDTO data) {
        TreatmentBinding entity = new TreatmentBinding();
        BeanUtils.copyProperties(data, entity);
        if (entity.getStatus() == null) {
            entity.setStatus(1);
        }
        fillTreatmentAudit(entity, true);
        treatmentMapper.insert(entity);
        return JsonVO.success(entity.getId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> addUsageBinding(UsageBindingDTO data) {
        UsageBinding entity = new UsageBinding();
        BeanUtils.copyProperties(data, entity);
        if (entity.getStatus() == null) {
            entity.setStatus(1);
        }
        fillUsageAudit(entity, true);
        usageMapper.insert(entity);
        return JsonVO.success(entity.getId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> modifyTreatmentBinding(TreatmentBindingDTO data) {
        if (!StringUtils.hasText(data.getId())) {
            throw new BusinessException("ID不能为空");
        }
        TreatmentBinding entity = treatmentMapper.selectById(data.getId());
        if (entity == null) {
            throw new BusinessException("诊疗绑定记录不存在");
        }
        BeanUtils.copyProperties(data, entity);
        fillTreatmentAudit(entity, false);
        treatmentMapper.updateById(entity);
        return JsonVO.success(entity.getId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> modifyUsageBinding(UsageBindingDTO data) {
        if (!StringUtils.hasText(data.getId())) {
            throw new BusinessException("ID不能为空");
        }
        UsageBinding entity = usageMapper.selectById(data.getId());
        if (entity == null) {
            throw new BusinessException("用法绑定记录不存在");
        }
        BeanUtils.copyProperties(data, entity);
        fillUsageAudit(entity, false);
        usageMapper.updateById(entity);
        return JsonVO.success(entity.getId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> deleteTreatmentBinding(String id) {
        TreatmentBinding entity = treatmentMapper.selectById(id);
        if (entity == null) {
            throw new BusinessException("诊疗绑定记录不存在");
        }
        treatmentMapper.deleteById(id);
        return JsonVO.success(id);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> deleteUsageBinding(String id) {
        UsageBinding entity = usageMapper.selectById(id);
        if (entity == null) {
            throw new BusinessException("用法绑定记录不存在");
        }
        usageMapper.deleteById(id);
        return JsonVO.success(id);
    }

    private void fillTreatmentAudit(TreatmentBinding entity, boolean isInsert) {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                entity.setTenantId(currentUser.getTenantId());
                if (isInsert) {
                    entity.setCreatedBy(currentUser.getId());
                }
                entity.setUpdatedBy(currentUser.getId());
            }
        } catch (Exception ignored) {
        }
    }

    private void fillUsageAudit(UsageBinding entity, boolean isInsert) {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                entity.setTenantId(currentUser.getTenantId());
                if (isInsert) {
                    entity.setCreatedBy(currentUser.getId());
                }
                entity.setUpdatedBy(currentUser.getId());
            }
        } catch (Exception ignored) {
        }
    }
}
