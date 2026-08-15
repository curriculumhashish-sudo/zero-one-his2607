package com.zeroone.star.basedata.service.impl;

import com.alibaba.nacos.common.utils.StringUtils;
import com.zeroone.star.basedata.constant.DefaultConstant;
import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.basedata.entity.EnumValue;
import com.zeroone.star.basedata.repository.EnumTypeRepository;
import com.zeroone.star.basedata.repository.EnumValueRepository;
import com.zeroone.star.basedata.service.EnumTypeService;
import com.zeroone.star.basedata.service.EnumValueService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.BeanUtils;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.data.mongodb.core.query.Update;
import org.bson.Document;
import java.util.Date;
import java.util.List;

@Slf4j
@Service
public class EnumTypeServiceImpl implements EnumTypeService {

    @Resource
    private EnumTypeRepository enumTypeRepository;

    @Resource
    private EnumValueRepository enumValueRepository;
    @Resource
    MongoTemplate template;
    @Resource
    private UserHolder userHolder;

    // TODO: redis上对于租户id是怎么存储的？
    @Resource
    private RedisTemplate<String, Object> redisTemplate;
    @Resource
    private StringRedisTemplate stringRedisTemplate;
    private String getCurrentUserTenantId(String userId) {

        Object tenantInfo = stringRedisTemplate.opsForHash().get("user:session:" + userId, "tenant_id");


        if (tenantInfo == null) {
            log.warn("租户信息为空，使用默认租户id");
            return DefaultConstant.DEFAULT_TENANT_ID;
        }
        String tenantId = tenantInfo.toString();
        log.info("租户id:\t {}", tenantId);
        return tenantId;
    }


    private String getCurrentUserId() {
        String currentUserId;
        try {
            currentUserId = userHolder.getCurrentUser().getId();
            log.info("当前用户id:\t {}", currentUserId);
        } catch (Exception e) {
            log.warn("使用 UserHolder 失败:\t" + e.getMessage());
            currentUserId = DefaultConstant.DEFAULT_USER_ID;
        }
        return currentUserId;
    }

    @Override
    public PageDTO<EnumTypeDTO> queryAllEnumType(EnumTypeQuery query) {

        String tenantId = getCurrentUserTenantId(getCurrentUserId());

        Pageable pageable = PageRequest.of(
                (int) query.getPageIndex() - 1,
                (int) query.getPageSize(),
                Sort.by(Sort.Direction.DESC, "updatedAt")
        );

        Page<EnumTypeDTO> page;
        if (query.getEnumTypeKeyword() != null) {
            page = enumTypeRepository.findByKeyword(
                    tenantId,
                    query.getEnumTypeKeyword(),
                    pageable
            );
        } else {
            page = enumTypeRepository.findByTenantIdAndIsDeletedFalse(tenantId, pageable);
        }

        return EnumTypeDTO.create(page);
    }

    @Override
    public void saveEnumType(EnumTypeAddDTO dto) {

        String currentUserId = getCurrentUserId();
        String tenantId = getCurrentUserTenantId(currentUserId);

        if (enumTypeRepository.existsByEnumTypeKey(tenantId, dto.getEnumTypeKey())) {
            throw new RuntimeException("枚举类型键已经存在");
        }

        EnumType enumType = new EnumType();
        BeanUtils.copyProperties(dto, enumType);

        enumType.setId(dto.getEnumTypeKey() + "_" + tenantId);

        enumType.setTenantId(tenantId);
        enumType.setCreatedAt(new Date());
        enumType.setCreatedBy(currentUserId);
        enumType.setIsDeleted(false);

        enumTypeRepository.save(enumType);
    }

    @Override
    public void updateEnumType(EnumTypeModifyDTO dto) {

        String currentUserId = getCurrentUserId();
        String tenantId = getCurrentUserTenantId(currentUserId);

        EnumType existing = enumTypeRepository.findById(dto.getId())
                .orElseThrow(() -> new RuntimeException("枚举类型不存在"));

        if (Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new RuntimeException("枚举类型已删除");
        }

        if (!existing.getTenantId().equals(tenantId)) {
            throw new RuntimeException("枚举类型不属于当前租户");
        }

        // 如果枚举类型键修改了，并且新的枚举类型键已经存在，则抛出异常
        if (!existing.getEnumTypeKey().equals(dto.getEnumTypeKey())
                && enumTypeRepository.existsByEnumTypeKey(tenantId, dto.getEnumTypeKey())) {
            throw new RuntimeException("枚举类型键已经存在");
        }

        BeanUtils.copyProperties(dto, existing);
        existing.setUpdatedAt(new Date());
        existing.setUpdatedBy(currentUserId);

        enumTypeRepository.save(existing);
    }

    @Override
    public void deleteEnumType(String id) {

        String currentUserId = getCurrentUserId();
        String tenantId = getCurrentUserTenantId(currentUserId);


        EnumType existing = enumTypeRepository.findById(id)
                .orElseThrow(() -> new RuntimeException("枚举类型不存在"));

        if (Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new RuntimeException("枚举类型已删除");
        }

        if (!existing.getTenantId().equals(tenantId)) {
            throw new RuntimeException("枚举类型不属于当前租户");
        }

        // 如果是系统枚举类型，则不能删除
        if (existing.getIsSystem()) {
            throw new RuntimeException("系统枚举类型不能删除");
        }

        // 逻辑删除枚举类型
        existing.setIsDeleted(true);
        existing.setUpdatedAt(new Date());
        existing.setUpdatedBy(currentUserId);
        enumTypeRepository.save(existing);

        List<EnumValue> enumValues = enumValueRepository.findByEnumTypeId(id);
        for (EnumValue enumValue : enumValues) {
            enumValue.setIsDeleted(true);
            enumValue.setUpdatedAt(new Date());
            enumValue.setUpdatedBy(currentUserId);
            enumValueRepository.save(enumValue);
        }
    }
}
