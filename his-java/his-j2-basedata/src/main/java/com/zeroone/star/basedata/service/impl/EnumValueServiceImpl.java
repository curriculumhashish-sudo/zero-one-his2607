package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.repository.EnumTypeRepository;
import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.basedata.entity.EnumValue;
import com.zeroone.star.basedata.repository.EnumValueRepository;
import com.zeroone.star.basedata.service.EnumValueService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.BeanUtils;
import org.bson.Document;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageImpl;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.aggregation.Aggregation;
import org.springframework.data.mongodb.core.aggregation.AggregationOperation;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;


@Slf4j
@Service
public class EnumValueServiceImpl implements EnumValueService {

    @Resource
    private UserHolder userHolder;

    @Resource
    private RedisTemplate<String, Object> redisTemplate;

    @Resource
    private EnumValueRepository enumValueRepository;

    @Resource
    private EnumTypeRepository enumTypeRepository;

    @Resource
    MongoTemplate mongoTemplate;

    private String getTenantId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            String userId = currentUser.getId();
            Object tenantIdObj = redisTemplate.opsForValue().get("user:session:" + userId);
            if (tenantIdObj != null) {
                return tenantIdObj.toString();
            }
            log.warn("Redis中未找到用户[{}]的租户ID，使用默认值", userId);
        } catch (Exception e) {
            log.warn("获取租户ID失败，使用默认值", e);
        }
        return "1";
    }

    @Override
    public void saveEnum(EnumAddDTO dto) {
        String tenantId = getTenantId();
        if (!enumTypeRepository.existsById(dto.getEnumTypeId())) {
            throw new RuntimeException("枚举类型不存在");
        }

        if (enumValueRepository.existsByEnumTypeIdAndEnumKey(tenantId, dto.getEnumTypeId(), dto.getEnumKey())) {
            throw new RuntimeException("同一枚举类型下枚举键已存在");
        }

        if (enumValueRepository.existsByEnumTypeIdAndEnumCode(tenantId, dto.getEnumTypeId(), dto.getEnumCode())) {
            throw new RuntimeException("同一枚举类型下枚举码已存在");
        }
        if (enumValueRepository.existsByEnumTypeIdAndDisplay(tenantId, dto.getEnumTypeId(), dto.getDisplay())) {
            throw new RuntimeException("同一枚举类型下标签已存在，不能同名");
        }

        EnumValue enumValue = new EnumValue();
        BeanUtils.copyProperties(dto, enumValue);

        enumValue.setTenantId(tenantId);
        enumValue.setIsDeleted(false);
        enumValue.setCreatedAt(new Date());
        enumValue.setCreatedBy("test");

        enumValueRepository.save(enumValue);
    }

    @Override
    public void updateEnum(EnumModifyDTO dto) {
        String tenantId = getTenantId();
        EnumValue existing = enumValueRepository.findById(dto.getId())
                .orElseThrow(() -> new RuntimeException("枚举值不存在"));

        if (Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new RuntimeException("枚举值已删除");
        }

        if (!existing.getEnumTypeId().equals(dto.getEnumTypeId())) {
            throw new RuntimeException("枚举值不属于当前枚举类型");
        }

        if (!existing.getTenantId().equals(tenantId)) {
            throw new RuntimeException("枚举值不属于当前租户");
        }

        if (enumValueRepository.existsByEnumTypeIdAndDisplay(
                tenantId, dto.getEnumTypeId(), dto.getDisplay())) {
            throw new RuntimeException("同一枚举类型下标签已存在，不能同名");
        }

        existing.setDisplay(dto.getDisplay());
        existing.setOrder(dto.getOrder());
        existing.setStatus(dto.getStatus());
        existing.setUpdatedAt(new Date());
        existing.setUpdatedBy("test");

        enumValueRepository.save(existing);
    }

    @Override
    public void deleteEnum(String enumValueId) {
        String tenantId = getTenantId();
        EnumValue existing = enumValueRepository.findById(enumValueId)
                .orElseThrow(() -> new RuntimeException("枚举值不存在"));
        if (Boolean.TRUE.equals(existing.getIsDeleted())) {
            throw new RuntimeException("枚举值已删除");
        }
        if (!existing.getTenantId().equals(tenantId)) {
            throw new RuntimeException("枚举值不属于当前租户");
        }

        // 逻辑删除
        existing.setIsDeleted(true);
        enumValueRepository.save(existing);
    }


    @Override
    public Map<String, List<EnumDTO>> initTableEnums(List<String> enumTypeKeys) {
        String tenantId = getTenantId();
        Map<String, List<EnumDTO>> enumTypeMap = new HashMap<>();

        for (String enumTypeKey : enumTypeKeys) {
            EnumType enumType = enumTypeRepository.findActiveByEnumTypeKey(tenantId, enumTypeKey);
            if (enumType == null) {
                // TODO: 这里需要对为查找到相关类型的情况做出处理，是否应该返回错误？
                // enumTypeMap.put(enumTypeKey, new ArrayList<EnumDTO>());

                throw new RuntimeException("枚举类型不存在: " + enumTypeKey);
                // continue;
            }
            String enumTypeId = enumType.getId();
            List<EnumDTO> enumDTOList = EnumValue.toDTOList(
                    enumValueRepository.findByEnumTypeIdAndIsDeletedFalse(tenantId, enumTypeId));
            enumTypeMap.put(enumTypeKey, enumDTOList);
        }

        return enumTypeMap;
    }

    @Override
    public JsonVO<List<String>> getEnumNameList(String id) {
        if(StringUtils.hasText(id)){
            List<String> displays = new ArrayList<>();
            List<EnumValue> enumValues = enumTypeRepository.findByEnumTypeId(id);
            enumValues.forEach(enumValue -> {displays.add(enumValue.getDisplay());});
            return JsonVO.success(displays);
        }
        return JsonVO.fail(null);
    }

    @Override
    public JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query) {

        //构建分页对象
        int pageIndex = Math.max(0, query.getPageIndex() - 1);
        PageRequest pageRequest = PageRequest.of(
                pageIndex,
                query.getPageSize(),
                Sort.by(Sort.Direction.ASC, "order")
        );


        //执行查询
        String enumCode = query.getEnumCode();
        String enumTypeId = query.getEnumTypeId();
        String display = query.getDisplay();
        String tenantId = getTenantId();
        Page<EnumValue> enumValues = findByQuery(
                tenantId,
                enumCode,
                enumTypeId,
                display,
                pageRequest
        );

        //返回结果
        PageDTO<EnumDTO> pageDTO = EnumValue.toPageDTO(enumValues);
        pageDTO.setEnumTypeId(enumTypeId);
        return JsonVO.success(pageDTO);
    }

    public Page<EnumValue> findByQuery(String tenantId, String enumCode,
                                       String enumTypeId, String display,
                                       PageRequest pageRequest) {

        if (!StringUtils.hasText(enumTypeId)) { // 枚举类型id为空时，返回空结果
            return new PageImpl<>(List.of(), pageRequest, 0);
        }

        // ========== 1. 构建 enum_value 侧的动态过滤条件 ==========
        List<Criteria> valueFilters = new ArrayList<>();
        if (StringUtils.hasText(enumCode)) {
            try {
                //添加动态条件
                int code = Integer.parseInt(enumCode);
                valueFilters.add(Criteria.where("enumCode").is(code));
            } catch (NumberFormatException ignored) { // 枚举编码为空时则返回空结果
                return new PageImpl<>(List.of(), pageRequest, 0);
            }
        }
        //display模糊查询构造条件
        if (StringUtils.hasText(display)) {
            valueFilters.add(Criteria.where("display").regex(display, "i"));
        }

        // ========== 2. 构建聚合管道（主集合: EnumType） ==========
        List<AggregationOperation> operations = new ArrayList<>();

        // 2.1 $addFields: 将 _id 转为字符串 _idStr（解决 ObjectId/String 类型不匹配）

        //自定义一个聚合操作,新增加一个字符串类型字段_idStr,存储_id转化为字符串的内容
        operations.add(ctx -> new Document("$addFields",
                new Document("_idStr", new Document("$toString", "$_id"))));

        // 2.2 $match: 用 _idStr（String）匹配 enumTypeId（String），类型统一
        Criteria typeCriteria = Criteria.where("tenantId").is(tenantId)
                .and("isDeleted").is(false)
                .and("_idStr").is(enumTypeId);
        operations.add(Aggregation.match(typeCriteria));

        // 2.3 $lookup: EnumType._idStr → EnumValue.enumTypeId（都是 String）
        operations.add(Aggregation.lookup("EnumValue", "_idStr", "enumTypeId", "enumValueList"));

        // 2.4 $unwind: 展开枚举值数组，无匹配则丢弃
        operations.add(Aggregation.unwind("enumValueList", false));

        // 2.5 $replaceRoot: 以 EnumValue 作为根文档
        operations.add(Aggregation.replaceRoot("enumValueList"));

        // 2.6 过滤 EnumValue 侧的 isDeleted 和 tenantId
        operations.add(Aggregation.match(Criteria.where("isDeleted").is(false)
                .and("tenantId").is(tenantId)));

        // 2.7 动态过滤 enumCode / display
        if (!valueFilters.isEmpty()) {
            operations.add(Aggregation.match(
                    new Criteria().andOperator(valueFilters.toArray(new Criteria[0]))));
        }

        // 2.8 $project: 只返回前端需要的 4 个字段
        operations.add(Aggregation.project("display", "enumCode", "order", "description","_id"));

        // 2.9 排序 + 分页
        operations.add(Aggregation.sort(pageRequest.getSort()));
        operations.add(Aggregation.skip(pageRequest.getOffset()));
        operations.add(Aggregation.limit(pageRequest.getPageSize()));

        // ========== 3. 执行分页查询 ==========
        Aggregation aggregation = Aggregation.newAggregation(operations);
        List<EnumValue> list = mongoTemplate.aggregate(aggregation, "EnumType", EnumValue.class)
                .getMappedResults();

        // ========== 4. 查总数 ==========
        List<AggregationOperation> countOps = new ArrayList<>(operations);
        countOps.remove(countOps.size() - 1); // limit
        countOps.remove(countOps.size() - 1); // skip
        countOps.remove(countOps.size() - 1); // sort
        countOps.add(Aggregation.count().as("total"));

        Aggregation countAggregation = Aggregation.newAggregation(countOps);
        long total = 0;
        Document resultDoc = mongoTemplate.aggregate(countAggregation, "EnumType", Document.class)
                .getUniqueMappedResult();
        if (resultDoc != null) {
            total = resultDoc.getInteger("total", 0);
        }

        // ========== 5. 返回 Page ==========
        return new PageImpl<>(list, pageRequest, total);
    }
}
