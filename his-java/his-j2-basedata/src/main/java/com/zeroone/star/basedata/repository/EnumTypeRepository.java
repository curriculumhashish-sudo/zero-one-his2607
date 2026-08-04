package com.zeroone.star.basedata.repository;


import com.zeroone.star.basedata.entity.EnumType;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.Query;


public interface EnumTypeRepository extends MongoRepository<EnumType, String> {

    @Query(value = "{ 'enumTypeKey': ?1, $or: [ { 'tenantId': ?0 }, { 'tenantId': 'sys_tenant' } ] }", exists = true)
    boolean existsByEnumTypeKey(String tenantId, String enumTypeKey);

    /**
     * 根据关键字查询枚举类型，这里会额外查询默认系统租户的枚举类型
     * @param tenatId 枚举类型所属租户ID
     * @param keyword 关键字
     * @param pageable 分页信息
     * @return 枚举类型列表
     */
    @Query("{$and: ["
            + "{'tenantId': {$in: ['sys_tenant', ?0]}}, "
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


    String findEnumTypeIdByEnumTypeKey(String enumTypeKey);

    /**
     * 根据枚举类型关键字查询枚举类型ID，包含默认系统租户的枚举类型
     * @param tenantId 枚举类型所属租户ID
     * @param enumTypeKey 枚举类型关键字
     * @return 枚举类型ID
     */
    @Query("{'enumTypeKey': ?1, 'tenantId': {$in: ['sys_tenant', ?0]}, 'isDeleted': false}")
    EnumType findActiveByEnumTypeKey(String tenantId, String enumTypeKey);

}
