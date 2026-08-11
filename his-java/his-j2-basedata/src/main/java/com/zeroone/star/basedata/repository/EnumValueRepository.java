package com.zeroone.star.basedata.repository;

import com.zeroone.star.basedata.entity.EnumValue;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumSimpleDTO;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.Query;

import java.util.List;

public interface EnumValueRepository extends MongoRepository<EnumValue, String> {

    List<EnumValue> findByEnumTypeId(String enumTypeId);

    @Query(
            value = "{ 'enumTypeId': ?1, 'isDeleted': false, $or: [ { 'tenantId': ?0 }, { 'tenantId': 'sys_tenant' } ] }",
            sort = "{ 'order': 1 }"  // 1 表示升序，-1 表示降序
    )
    List<EnumValue> findByEnumTypeIdAndIsDeletedFalse(String tenantId, String enumTypeId);

    @Query(value = "{ 'enumTypeId': ?1, 'enumKey': ?2, $or: [ { 'tenantId': ?0 }, { 'tenantId': 'sys_tenant' } ] }", exists = true)
    boolean existsByEnumTypeIdAndEnumKey(String tenantId, String enumTypeId, String enumKey);

    @Query(value = "{ 'enumTypeId': ?1, 'enumCode': ?2, $or: [ { 'tenantId': ?0 }, { 'tenantId': 'sys_tenant' } ] }", exists = true)
    boolean existsByEnumTypeIdAndEnumCode(String tenantId, String enumTypeId, Integer enumCode);

    @Query(value = "{ 'enumTypeId': ?1, 'display': ?2, $or: [ { 'tenantId': ?0 }, { 'tenantId': 'sys_tenant' } ] }", exists = true)
    boolean existsByEnumTypeIdAndDisplay(String tenantId, String enumTypeId, String display);

    @Query("{ 'enumTypeId': ?1, 'isDeleted': false, $or: [ { 'tenantId': ?0 }, { 'tenantId': 'sys_tenant' } ] }")
    List<EnumValue> findByTenantIdAndEnumTypeIdAndIsDeletedFalse(String tenantId, String enumTypeId);

    @Query("{ 'enumTypeId': ?0, 'isDeleted': false, $or: [ { 'tenantId': ?0 }, { 'tenantId': 'sys_tenant' } ] }")
    List<EnumValue> findSystemEnumValuesByEnumTypeId(String enumTypeId);
}
