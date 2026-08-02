package com.zeroone.star.basedata.service;

import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;

public interface EnumTypeService {

    public PageDTO<EnumTypeDTO> queryAllEnumType(EnumTypeQuery query);

    public void saveEnumType(EnumTypeAddDTO dto);

    public void updateEnumType(EnumTypeModifyDTO dto);

    public JsonVO<String> deleteEnumType(String enumTypeId);
}
