package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.configuration.entity.DocumentTypeDO;
import com.zeroone.star.configuration.mapper.DocumentTypeMapper;
import com.zeroone.star.configuration.service.DocumentTypeService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.documenttype.DocumentTypeAddDTO;
import com.zeroone.star.project.dto.documenttype.DocumentTypeUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;
import jakarta.annotation.Resource;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;

/**
 * 文书类型服务实现类
 */
@Service
@RequiredArgsConstructor
public class DocumentTypeServiceImpl implements DocumentTypeService {
    
    private final DocumentTypeMapper documentTypeMapper;
    
    @Resource
    private UserHolder userHolder;
    
    @Override
    public JsonVO<Void> add(DocumentTypeAddDTO dto) {
        // 检查编码是否已存在
        LambdaQueryWrapper<DocumentTypeDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(DocumentTypeDO::getCode, dto.getCode());
        if (documentTypeMapper.selectCount(wrapper) > 0) {
            throw new RuntimeException("文书类型编码已存在");
        }
        
        // 构建实体
        DocumentTypeDO entity = new DocumentTypeDO();
        BeanUtils.copyProperties(dto, entity);
        entity.setIsEnabled(dto.getIsEnabled() != null ? dto.getIsEnabled() : 1);
        entity.setSortOrder(dto.getSortOrder() != null ? dto.getSortOrder() : 0);
        
        // 设置审计字段
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                entity.setCreatedBy(currentUser.getId());
                entity.setUpdatedBy(currentUser.getId());
                entity.setTenantId(userHolder.getCurrentTenantId());
            }
        } catch (Exception e) {
            // 如果获取用户信息失败，忽略审计字段填充
        }
        
        // 插入数据库
        documentTypeMapper.insert(entity);
        
        return JsonVO.success(null);
    }
    
    @Override
    public JsonVO<Void> update(DocumentTypeUpdateDTO dto) {
        // 检查是否存在
        DocumentTypeDO entity = documentTypeMapper.selectById(dto.getId());
        if (entity == null) {
            throw new RuntimeException("文书类型不存在");
        }
        
        // 如果修改了编码，检查新编码是否已被使用
        if (!entity.getCode().equals(dto.getCode())) {
            LambdaQueryWrapper<DocumentTypeDO> wrapper = new LambdaQueryWrapper<>();
            wrapper.eq(DocumentTypeDO::getCode, dto.getCode())
                   .ne(DocumentTypeDO::getId, dto.getId());
            if (documentTypeMapper.selectCount(wrapper) > 0) {
                throw new RuntimeException("文书类型编码已存在");
            }
        }
        
        // 更新实体
        BeanUtils.copyProperties(dto, entity);
        
        // 设置更新人
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                entity.setUpdatedBy(currentUser.getId());
            }
        } catch (Exception e) {
            // 如果获取用户信息失败，忽略审计字段填充
        }
        
        documentTypeMapper.updateById(entity);
        
        return JsonVO.success(null);
    }
    
    @Override
    public JsonVO<Void> delete(String id) {
        // 检查是否存在
        DocumentTypeDO entity = documentTypeMapper.selectById(id);
        if (entity == null) {
            throw new RuntimeException("文书类型不存在");
        }
        
        // TODO: 删除前检查是否有文书模板引用此类型
        // 需要创建EmrTemplate实体和Mapper后启用以下代码
        // LambdaQueryWrapper<EmrTemplateDO> templateWrapper = new LambdaQueryWrapper<>();
        // templateWrapper.eq(EmrTemplateDO::getDocumentTypeCode, entity.getCode());
        // if (emrTemplateMapper.selectCount(templateWrapper) > 0) {
        //     throw new RuntimeException("该文书类型已被模板引用，无法删除");
        // }
        
        // 设置删除审计字段
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                entity.setDeletedAt(LocalDateTime.now());
                entity.setDeletedBy(currentUser.getId());
            }
        } catch (Exception e) {
            // 如果获取用户信息失败，忽略审计字段填充
        }
        
        // 软删除
        documentTypeMapper.deleteById(id);
        
        return JsonVO.success(null);
    }
}
