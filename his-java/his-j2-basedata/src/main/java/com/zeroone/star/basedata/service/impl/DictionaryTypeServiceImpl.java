package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.exception.DictionaryBusinessException;
import com.zeroone.star.basedata.mapper.DictionaryTypeMapper;
import com.zeroone.star.basedata.service.DictionaryTypeService;
import com.zeroone.star.basedata.entity.TerCodeSystem;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeCreateDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDeleteDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeUpdateDTO;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

@Service
public class DictionaryTypeServiceImpl implements DictionaryTypeService {

    @Autowired
    private DictionaryTypeMapper dictionaryTypeMapper;

    @Override  //获取字典类型列表（条件+分页）
    public PageDTO<DictionaryTypeDTO> queryDictionarType(DictionaryTypeQuery query) {
        LambdaQueryWrapper<TerCodeSystem> lambdaQueryWrapper = new LambdaQueryWrapper<>();

        lambdaQueryWrapper.select(
                TerCodeSystem::getId,
                TerCodeSystem::getName,
                TerCodeSystem::getTitle
        );
        lambdaQueryWrapper.eq(TerCodeSystem::getIsDeleted,0);

        if(StringUtils.hasText(query.getDictKeyword())){
            String dictKeyword = query.getDictKeyword();

            lambdaQueryWrapper.and(
                    w -> w
                            .like(TerCodeSystem::getName,dictKeyword)
                            .or()
                            .like(TerCodeSystem::getTitle,dictKeyword)
            );
        }

        Page<TerCodeSystem> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<TerCodeSystem> result = dictionaryTypeMapper.selectPage(page, lambdaQueryWrapper);


        return PageDTO.create(result, item -> {
            DictionaryTypeDTO dto = new DictionaryTypeDTO();
            dto.setDictTypeId(item.getId());
            dto.setDictTypeCode(item.getTitle());
            dto.setDictTypeName(item.getName());
            return dto;
        });
    }


    @Override
    @Transactional(rollbackFor = Exception.class)
    public String createDictionaryType(DictionaryTypeCreateDTO data) {
        String name = data.getName().trim();
        if (dictionaryTypeMapper.countName(name, null) > 0) {
            throw new DictionaryBusinessException("编码类型编码已存在");
        }
        TerCodeSystem entity = new TerCodeSystem();
        entity.setName(name);
        entity.setTitle(data.getTitle().trim());
        entity.setDescription(normalize(data.getDescription()));
        entity.setVersion("1.0");
        entity.setStatus("2");
        entity.setTenantId("1");
        entity.setIsDeleted(0);
        if (dictionaryTypeMapper.insert(entity) != 1) {
            throw new DictionaryBusinessException("新增编码类型失败");
        }
        return entity.getId();
    }
    @Override
    @Transactional(rollbackFor = Exception.class)
    public String updateDictionaryType(DictionaryTypeUpdateDTO data) {
        requireActive(data.getId());
        String name = data.getName().trim();
        if (dictionaryTypeMapper.countName(name, data.getId()) > 0) {
            throw new DictionaryBusinessException("编码类型编码已存在");
        }
        TerCodeSystem entity = new TerCodeSystem();
        entity.setId(data.getId());
        entity.setName(name);
        entity.setTitle(data.getTitle().trim());
        entity.setDescription(normalize(data.getDescription()));
        if (dictionaryTypeMapper.updateDictionaryType(entity) != 1) {
            throw new DictionaryBusinessException("编码类型数据已发生变化，请刷新后重试");
        }
        return data.getId();
    }
    @Override
    @Transactional(rollbackFor = Exception.class)
    public String deleteDictionaryType(DictionaryTypeDeleteDTO data) {
        requireActive(data.getId());
        if (dictionaryTypeMapper.countConcepts(data.getId()) > 0) {
            throw new DictionaryBusinessException("该编码类型下存在字典项，不能删除");
        }
        TerCodeSystem entity = new TerCodeSystem();
        entity.setId(data.getId());
        entity.setIsDeleted(1);
        if (dictionaryTypeMapper.logicallyDeleteDictionaryType(entity) != 1) {
            throw new DictionaryBusinessException("编码类型数据已发生变化，请刷新后重试");
        }
        return data.getId();
    }
    private void requireActive(String id) {
        Long count = dictionaryTypeMapper.selectCount(new LambdaQueryWrapper<TerCodeSystem>()
                .eq(TerCodeSystem::getId, id)
                .eq(TerCodeSystem::getIsDeleted, 0));
        if (count == 0) {
            throw new DictionaryBusinessException("编码类型不存在或已删除");
        }
    }
    private String normalize(String value) {
        return StringUtils.hasText(value) ? value.trim() : null;
    }
}
