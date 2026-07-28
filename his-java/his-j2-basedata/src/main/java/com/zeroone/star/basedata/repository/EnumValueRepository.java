package com.zeroone.star.basedata.repository;

import com.zeroone.star.basedata.entity.EnumValue;
import org.springframework.data.mongodb.repository.MongoRepository;

import java.util.List;

public interface EnumValueRepository extends MongoRepository<EnumValue, String> {

    List<EnumValue> findByEnumTypeId(String enumTypeId);

    List<EnumValue> findByEnumTypeIdAndIsDeletedFalse(String enumTypeId);

    boolean existsByTenantIdAndEnumTypeIdAndEnumKey(String tenantId, String enumTypeId, String enumKey);

    boolean existsByTenantIdAndEnumTypeIdAndEnumCode(String tenantId, String enumTypeId, Integer enumCode);

    boolean existsByTenantIdAndEnumTypeIdAndDisplay(String tenantId, String enumTypeId, String display);
}
