package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.EnumValue;
import com.zeroone.star.basedata.repository.EnumTypeRepository;
import com.zeroone.star.basedata.repository.EnumValueRepository;
import com.zeroone.star.basedata.service.EnumTypeService;
import com.zeroone.star.basedata.service.EnumValueService;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumModifyDTO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
public class EnumValueServiceImpl implements EnumValueService {

    @Resource
    private EnumValueRepository enumValueRepository;

    @Resource
    private EnumTypeRepository enumTypeRepository;

    @Override
    public void saveEnum(EnumAddDTO enumAddDTO) {

    }

    @Override
    public void updateEnum(EnumModifyDTO dto) {
        EnumValue existing = enumValueRepository.findById(dto.getId())
                .orElseThrow(() -> new RuntimeException("枚举值不存在"));

        if (Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new RuntimeException("枚举值已删除");
        }

        existing.setDisplay(dto.getLabel());
        existing.setOrder(dto.getOrder());
        existing.setStatus(dto.getStatus());
        existing.setUpdatedAt(new Date());
        existing.setUpdatedBy("test");

        enumValueRepository.save(existing);
    }

    @Override
    public void deleteEnum(String enumValueId) {

    }


    @Override
    public Map<String, List<EnumDTO>> initTableEnums(List<String> enumTypeKeys) {
        Map<String, List<EnumDTO>> enumTypeMap = new HashMap<>();

        for (String enumTypeKey : enumTypeKeys) {
            String enumTypeId = enumTypeRepository.findActiveEnumTypeIdByEnumTypeKey("1", enumTypeKey);
            if (enumTypeId != null) {
                List<EnumDTO> enumDTOList = EnumValue.toDTOList(
                        enumValueRepository.findByEnumTypeIdAndIsDeletedFalse(enumTypeId));
                enumTypeMap.put(enumTypeKey, enumDTOList);
            }
        }

        return enumTypeMap;
    }
}
