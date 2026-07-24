package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.mapper.EnumValueMapper;
import com.zeroone.star.basedata.service.EnumValueService;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import jakarta.annotation.Resource;

import java.util.List;

public class EnumValueServiceImpl implements EnumValueService {

    @Resource
    private EnumValueMapper enumValueMapper;

    @Override
    public void saveEnum(EnumModifyDTO enumModifyDTO) {

    }

    @Override
    public void updateEnum(EnumModifyDTO enumModifyDTO) {

    }

    @Override
    public void deleteEnum(String enumValueId) {

    }

    @Override
    public List<EnumDTO> listEnumsByEnumType(String enumType) {
        return List.of();
    }
}
