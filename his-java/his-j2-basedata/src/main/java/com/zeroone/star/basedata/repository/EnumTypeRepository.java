package com.zeroone.star.basedata.repository;

import com.zeroone.star.basedata.constant.DefaultConstant;
import com.zeroone.star.basedata.entity.EnumValue;
import org.springframework.data.mongodb.repository.Aggregation;
import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.Query;

import java.util.List;
import org.springframework.data.mongodb.repository.Query;
public interface EnumTypeRepository extends MongoRepository<EnumType, String> {

    @Query(value = "{ 'enumTypeKey': ?1, $or: [ { 'tenantId': ?0 }, { 'tenantId': "+ DefaultConstant.DEFAULT_TENANT_ID +" } ] }", exists = true)
    boolean existsByEnumTypeKey(String tenantId, String enumTypeKey);
    @Aggregation(pipeline = {
            "{$addFields: {_idStr: {$toString: '$_id'}}}",
            "{$match: {_idStr: ?0}}",
            "{$lookup: {from: 'EnumValue', localField: '_idStr', foreignField: 'enumTypeId', as: 'enum_List'}}",
            "{$unwind: {path: '$enum_List', preserveNullAndEmptyArrays: false}}",
            "{$replaceRoot: {newRoot: '$enum_List'}}"
    })
    List<EnumValue> findByEnumTypeId(String id);

    boolean existsByTenantIdAndEnumTypeKey(String tenantId, String enumTypeKey);

    /**
     * 根据关键字查询枚举类型，这里会额外查询默认系统租户的枚举类型
     * @param tenatId 枚举类型所属租户ID
     * @param keyword 关键字
     * @param pageable 分页信息
     * @return 枚举类型列表
     */
    @Query("{$and: ["
            + "{'tenantId': {$in: ["+ DefaultConstant.DEFAULT_TENANT_ID +", ?0]}}, "
            + "{'isDeleted': false}, "
            + "{$or: ["
            + "{'enumTypeKey': {$regex: ?1, $options: 'i'}}, "
            + "{'displayName': {$regex: ?1, $options: 'i'}}, "
            + "]}"
            + "]}")
    Page<EnumTypeDTO> findByKeyword(
            String tenatId,
            String keyword,
            Pageable pageable);


    @Query("{'tenantId': {$in: ["+ DefaultConstant.DEFAULT_TENANT_ID +", ?0]}, 'isDeleted': false}")
    Page<EnumTypeDTO> findByTenantIdAndIsDeletedFalse(String tenantId, Pageable pageable);

    /**
     * 根据枚举类型关键字查询枚举类型ID，包含默认系统租户的枚举类型
     * @param tenantId 枚举类型所属租户ID
     * @param enumTypeKey 枚举类型关键字
     * @return 枚举类型ID
     */
    @Query("{'enumTypeKey': ?1, 'tenantId': {$in: ["+ DefaultConstant.DEFAULT_TENANT_ID +", ?0]}, 'isDeleted': false}")
    EnumType findActiveByEnumTypeKey(String tenantId, String enumTypeKey);

}
