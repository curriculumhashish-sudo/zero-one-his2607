package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.basedata.repository.EnumTypeRepository;
import com.zeroone.star.basedata.service.EnumTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.stereotype.Service;

import java.util.Date;

@Service
public class EnumTypeServiceImpl implements EnumTypeService {

    // TODO: 临时设定，后续需要从jwt中获取
    private static final String TENANT_ID = "sys_tenant";

    @Resource
    private EnumTypeRepository enumTypeRepository;

    @Override
    public PageDTO<EnumTypeDTO> queryAllEnumType(EnumTypeQuery query) {

        Pageable pageable = PageRequest.of(
                (int) query.getPageIndex() - 1,
                (int) query.getPageSize(),
                Sort.by(Sort.Direction.DESC, "updatedAt")
        );

        Page<EnumType> page = enumTypeRepository.findByKeyword(
                TENANT_ID,
                query.getEnumTypeKeyword(),
                pageable
        );

        return EnumType.toPageDTO(page);
    }

    @Override
    public void saveEnumType(EnumTypeAddDTO dto) {

        if (enumTypeRepository.existsByEnumTypeKey(TENANT_ID, dto.getEnumTypeKey())) {
            throw new RuntimeException("枚举类型键已经存在");
        }

        EnumType enumType = new EnumType();
        BeanUtils.copyProperties(dto, enumType);
        // TODO: 是否需要特别设定其id字符串？

        // TODO: 暂时设定
        enumType.setTenantId(TENANT_ID);
        enumType.setCreatedAt(new Date());
        enumType.setCreatedBy("test");
        enumType.setIsDeleted(false);

        enumTypeRepository.save(enumType);
    }

    @Override
    public void updateEnumType(EnumTypeModifyDTO dto) {

        EnumType existing = enumTypeRepository.findById(dto.getId())
                .orElseThrow(() -> new RuntimeException("枚举类型不存在"));

        if (Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new RuntimeException("枚举类型已删除");
        }

        if (!existing.getTenantId().equals(TENANT_ID)) {
            throw new RuntimeException("枚举类型不属于当前租户");
        }

        // 如果枚举类型键修改了，并且新的枚举类型键已经存在，则抛出异常
        if (!existing.getEnumTypeKey().equals(dto.getEnumTypeKey())
                && enumTypeRepository.existsByEnumTypeKey(TENANT_ID, dto.getEnumTypeKey())) {
            throw new RuntimeException("枚举类型键已经存在");
        }

        BeanUtils.copyProperties(dto, existing);
        existing.setUpdatedAt(new Date());
        existing.setUpdatedBy("test");

        enumTypeRepository.save(existing);
    }

    @Override
    public void removeEnumType(String enumTypeId) {

    }
}
