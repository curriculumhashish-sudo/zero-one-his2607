package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;

import java.util.List;
import java.util.Map;

public interface EnumValueService {

    public void saveEnum(EnumAddDTO enumAddDTO);

    public void updateEnum(EnumModifyDTO enumModifyDTO);

    public void deleteEnum(String enumValueId);

    public Map<String, List<EnumDTO>> initTableEnums(List<String> enumTypeKeys);
}
