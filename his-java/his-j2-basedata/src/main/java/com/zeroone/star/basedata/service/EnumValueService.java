package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumModifyDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumSimpleDTO;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;

import com.zeroone.star.project.dto.PageDTO;
import java.util.List;
import java.util.Map;

public interface EnumValueService {

    public void saveEnum(EnumAddDTO enumAddDTO);

    public void updateEnum(EnumModifyDTO enumModifyDTO);

    public void deleteEnum(String enumValueId);

    public Map<String, List<EnumDTO>> initTableEnums(List<String> enumTypeKeys);

    JsonVO<List<String>> getEnumNameList(String id);

    JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query);
}
