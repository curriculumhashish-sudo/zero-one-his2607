package com.zeroone.star.basedata.repository;

import com.zeroone.star.basedata.entity.EnumType;
import org.springframework.data.mongodb.repository.MongoRepository;

public interface EnumTypeRepository extends MongoRepository<EnumType, String> {
}
