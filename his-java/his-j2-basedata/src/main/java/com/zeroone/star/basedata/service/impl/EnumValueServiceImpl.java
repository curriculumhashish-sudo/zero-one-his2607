package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.basedata.entity.EnumValue;
import com.zeroone.star.basedata.repository.EnumTypeRepository;
import com.zeroone.star.basedata.repository.EnumValueRepository;
import com.zeroone.star.basedata.service.EnumValueService;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumModifyDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumSimpleDTO;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;

import java.util.*;

@Service
public class EnumValueServiceImpl implements EnumValueService {

    // TODO: 临时设定，后续需要从jwt中获取
    private static final String TENANT_ID = "1";

    @Resource
    private EnumValueRepository enumValueRepository;

    @Resource
    private EnumTypeRepository enumTypeRepository;

    @Override
    public void saveEnum(EnumAddDTO dto) {
        if (!enumTypeRepository.existsById(dto.getEnumTypeId())) {
            throw new RuntimeException("枚举类型不存在");
        }

        if (enumValueRepository.existsByEnumTypeIdAndEnumKey(TENANT_ID, dto.getEnumTypeId(), dto.getEnumKey())) {
            throw new RuntimeException("同一枚举类型下枚举键已存在");
        }

        if (enumValueRepository.existsByEnumTypeIdAndEnumCode(TENANT_ID, dto.getEnumTypeId(), dto.getEnumCode())) {
            throw new RuntimeException("同一枚举类型下枚举码已存在");
        }
        if (enumValueRepository.existsByEnumTypeIdAndDisplay(TENANT_ID, dto.getEnumTypeId(), dto.getDisplay())) {
            throw new RuntimeException("同一枚举类型下标签已存在，不能同名");
        }

        // TODO: 是否需要特别设定其id字符串？

        EnumValue enumValue = new EnumValue();
        BeanUtils.copyProperties(dto, enumValue);

        enumValue.setTenantId(TENANT_ID);
        enumValue.setIsDeleted(false);
        enumValue.setCreatedAt(new Date());
        enumValue.setCreatedBy("test");

        enumValueRepository.save(enumValue);
    }

    @Override
    public void updateEnum(EnumModifyDTO dto) {
        EnumValue existing = enumValueRepository.findById(dto.getId())
                .orElseThrow(() -> new RuntimeException("枚举值不存在"));

        if (Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new RuntimeException("枚举值已删除");
        }

        if (!existing.getEnumTypeId().equals(dto.getEnumTypeId())) {
            throw new RuntimeException("枚举值不属于当前枚举类型");
        }

        if (!existing.getTenantId().equals(TENANT_ID)) {
            throw new RuntimeException("枚举值不属于当前租户");
        }

        if (enumValueRepository.existsByEnumTypeIdAndDisplay(
                TENANT_ID, dto.getEnumTypeId(), dto.getDisplay())) {
            throw new RuntimeException("同一枚举类型下标签已存在，不能同名");
        }


//        if (!existing.getEnumKey().equals(dto.getEnumKey()) || !existing.getEnumCode().equals(dto.getEnumCode())) {
//            throw new RuntimeException("枚举值键或枚举值码不可修改");
//        }

        existing.setDisplay(dto.getDisplay());
        existing.setOrder(dto.getOrder());
        existing.setStatus(dto.getStatus());
        existing.setUpdatedAt(new Date());
        existing.setUpdatedBy("test");

        enumValueRepository.save(existing);
    }

    @Override
    public void deleteEnum(String enumValueId) {
        EnumValue existing = enumValueRepository.findById(enumValueId)
                .orElseThrow(() -> new RuntimeException("枚举值不存在"));
        if (Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new RuntimeException("枚举值已删除");
        }
        if (!existing.getTenantId().equals(TENANT_ID)) {
            throw new RuntimeException("枚举值不属于当前租户");
        }

        // 逻辑删除
        existing.setIsDeleted(true);
        enumValueRepository.save(existing);
    }


    @Override
    public Map<String, List<EnumSimpleDTO>> initTableEnums(List<String> enumTypeKeys) {

        Map<String, List<EnumSimpleDTO>> enumTypeMap = new HashMap<>();

        for (String enumTypeKey : enumTypeKeys) {
            EnumType enumType = enumTypeRepository.findActiveByEnumTypeKey(TENANT_ID, enumTypeKey);
            if (enumType == null) {
                // TODO: 这里需要对为查找到相关类型的情况做出处理，是否应该返回错误？
                // enumTypeMap.put(enumTypeKey, new ArrayList<EnumDTO>());

                throw new RuntimeException("枚举类型不存在: " + enumTypeKey);
                // continue;
            }
            String enumTypeId = enumType.getId();
            List<EnumSimpleDTO> enumDTOList = EnumValue.toSimpleDTOList(
                    enumValueRepository.findByEnumTypeIdAndIsDeletedFalse(TENANT_ID, enumTypeId));
            enumTypeMap.put(enumTypeKey, enumDTOList);
        }

        return enumTypeMap;
    }
}
