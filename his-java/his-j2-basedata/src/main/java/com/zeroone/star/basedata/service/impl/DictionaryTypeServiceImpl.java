package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.mapper.DictionaryTypeMapper;
import com.zeroone.star.basedata.service.DictionaryTypeService;
import com.zeroone.star.basedata.entity.TerCodeSystem;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictionaryTypeDTO;
import com.zeroone.star.project.query.j2.basedata.DictionaryTypeQuery;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
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
}
