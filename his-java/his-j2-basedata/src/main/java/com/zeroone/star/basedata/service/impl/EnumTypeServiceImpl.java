package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.mapper.EnumTypeMapper;
import com.zeroone.star.basedata.service.EnumTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import jakarta.annotation.Resource;

public class EnumTypeServiceImpl implements EnumTypeService {

    @Resource
    private EnumTypeMapper enumTypeMapper;

    @Override
    public PageDTO<EnumTypeDTO> queryAllEnumType(EnumTypeQuery query) {
        return null;
    }

    @Override
    public void saveEnumType(EnumTypeAddDTO dto) {

    }

    @Override
    public void updateEnumType(EnumTypeModifyDTO dto) {

    }

    @Override
    public void removeEnumType(String enumTypeId) {

    }
}
