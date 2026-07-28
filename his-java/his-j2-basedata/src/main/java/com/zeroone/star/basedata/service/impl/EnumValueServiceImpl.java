package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.basedata.entity.EnumValue;
import com.zeroone.star.basedata.repository.EnumTypeRepository;
import com.zeroone.star.basedata.repository.EnumValueRepository;
import com.zeroone.star.basedata.service.EnumTypeService;
import com.zeroone.star.basedata.service.EnumValueService;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumModifyDTO;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;

import java.util.*;

@Service
public class EnumValueServiceImpl implements EnumValueService {

    // 默认系统租户ID，各个租户公用
    private static final String TENANT_ID = "1";

    @Resource
    private EnumValueRepository enumValueRepository;

    @Resource
    private EnumTypeRepository enumTypeRepository;

    @Override
    public void saveEnum(EnumAddDTO dto) {

        // 第二个是系统默认租户
        if (enumValueRepository.existsByTenantIdAndEnumTypeIdAndEnumKey(TENANT_ID, dto.getEnumTypeId(), dto.getEnumKey())
                && enumValueRepository.existsByTenantIdAndEnumTypeIdAndEnumKey("1", dto.getEnumTypeId(), dto.getEnumKey())) {
            throw new RuntimeException("同一枚举类型下枚举键已存在");
        }

        if (enumValueRepository.existsByTenantIdAndEnumTypeIdAndEnumCode(TENANT_ID, dto.getEnumTypeId(), dto.getEnumCode())
                && enumValueRepository.existsByTenantIdAndEnumTypeIdAndEnumCode("1", dto.getEnumTypeId(), dto.getEnumCode())) {
            throw new RuntimeException("同一枚举类型下枚举码已存在");
        }
        if (enumValueRepository.existsByTenantIdAndEnumTypeIdAndDisplay(TENANT_ID, dto.getEnumTypeId(), dto.getDisplay())
                && enumValueRepository.existsByTenantIdAndEnumTypeIdAndDisplay("1", dto.getEnumTypeId(), dto.getDisplay())) {
            throw new RuntimeException("同一枚举类型下标签已存在，不能同名");
        }

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

        BeanUtils.copyProperties(dto, existing);

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
            EnumType enumType = enumTypeRepository.findActiveByEnumTypeKey("1", enumTypeKey);
            if (enumType == null) {
                // TODO: 这里需要对为查找到相关类型的情况做出处理
                // 如果未找到，创建一个空列表并添加到结果中
                enumTypeMap.put(enumTypeKey, new ArrayList<EnumDTO>());
                continue;
            }
            String enumTypeId = enumType.getId();

            // TODO： 这里会查找到所有内容，如果枚举类型属于该租户，但是枚举值不属于该租户，则依旧会返回给用户
            List<EnumDTO> enumDTOList = EnumValue.toDTOList(
                    enumValueRepository.findByEnumTypeIdAndIsDeletedFalse(enumTypeId));
            enumTypeMap.put(enumTypeKey, enumDTOList);
        }

        return enumTypeMap;
    }
}
