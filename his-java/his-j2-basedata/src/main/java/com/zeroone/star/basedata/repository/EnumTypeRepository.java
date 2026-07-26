package com.zeroone.star.basedata.repository;


import com.zeroone.star.basedata.entity.EnumType;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.Query;



public interface EnumTypeRepository extends MongoRepository<EnumType, String> {

    boolean existsByTenantIdAndEnumTypeKey(String tenantId, String enumTypeKey);

    @Query("{$and: ["
            + "{'tenantId': ?0}, "
            + "{'isDeleted': false}, "
            + "{$or: ["
            + "{'enumTypeKey': {$regex: ?1, $options: 'i'}}, "
            + "{'displayName': {$regex: ?1, $options: 'i'}}, "
            + "{'description': {$regex: ?1, $options: 'i'}}"
            + "]}"
            + "]}")
    Page<EnumType> findByKeyword(
            String tenatId,
            String keyword,
            Pageable pageable);
}
