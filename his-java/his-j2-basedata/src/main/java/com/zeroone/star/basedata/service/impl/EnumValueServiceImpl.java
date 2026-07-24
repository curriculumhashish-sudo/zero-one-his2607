package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.repository.EnumValueRepository;
import com.zeroone.star.basedata.service.EnumValueService;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumModifyDTO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Map;

@Service
public class EnumValueServiceImpl implements EnumValueService {

    @Resource
    private EnumValueRepository enumValueRepository;

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

    @Override
    public Map<String, List<EnumDTO>> initTableEnums(List<String> enumTypeKeys) {
        return Map.of();
    }


}
