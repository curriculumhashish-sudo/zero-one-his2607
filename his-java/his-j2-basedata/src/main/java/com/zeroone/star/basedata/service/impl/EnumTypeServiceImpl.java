package com.zeroone.star.basedata.service.impl;

import com.alibaba.nacos.common.utils.StringUtils;
import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.basedata.repository.EnumTypeRepository;
import com.zeroone.star.basedata.service.EnumTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.aggregation.Aggregation;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.stereotype.Service;

import org.bson.Document;
import java.util.Date;
import java.util.List;

@Service
public class EnumTypeServiceImpl implements EnumTypeService {

    // TODO: 临时设定，后续需要从jwt中获取
    private static final String TENANT_ID = "sys_tenant";

    @Resource
    private EnumTypeRepository enumTypeRepository;

    @Resource
    MongoTemplate template;

    @Override
    public PageDTO<EnumTypeDTO> queryAllEnumType(EnumTypeQuery query) {

        Pageable pageable = PageRequest.of(
                (int) query.getPageIndex() - 1,
                (int) query.getPageSize(),
                Sort.by(Sort.Direction.DESC, "updatedAt")
        );

        Page<EnumTypeDTO> page = enumTypeRepository.findByKeyword(
                TENANT_ID,
                query.getEnumTypeKeyword(),
                pageable
        );

        return EnumTypeDTO.create(page);
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
    public JsonVO<String> deleteEnumType(String id) {
        if (!StringUtils.hasText(id)) {
            return JsonVO.fail(null);
        }

        // 1. 聚合管道：$addFields 将 _id 转 _idStr → $match → $lookup 关联 EnumValue
        Aggregation aggregation = Aggregation.newAggregation(
                ctx -> new Document("$addFields",
                        new Document("_idStr", new Document("$toString", "$_id"))),
                Aggregation.match(Criteria.where("_idStr").is(id)),
                Aggregation.lookup("EnumValue", "_idStr", "enumTypeId", "enumValueList")
        );

        List<Document> results = template.aggregate(aggregation, "EnumType", Document.class)
                .getMappedResults();

        if (results.isEmpty()) {
            return JsonVO.fail("枚举类型不存在");
        }

        // 2. 删除所有关联的 EnumValue（enumTypeId 本身就是 String，无类型问题）
        Query valueQuery = new Query(Criteria.where("enumTypeId").is(id));
        template.remove(valueQuery, "EnumValue");

        // 3. 删除 EnumType 自身（用聚合查到的 _id 精确删除）
        Object rawId = results.get(0).get("_id");
        Query typeQuery = new Query(Criteria.where("_id").is(rawId));
        template.remove(typeQuery, EnumType.class, "EnumType");

        return JsonVO.success("success");
    }


}
