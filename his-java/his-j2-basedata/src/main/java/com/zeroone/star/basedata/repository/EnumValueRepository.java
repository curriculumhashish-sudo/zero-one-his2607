package com.zeroone.star.basedata.repository;

import com.zeroone.star.basedata.entity.EnumValue;
import org.springframework.data.mongodb.repository.MongoRepository;

public interface EnumValueRepository extends MongoRepository<EnumValue, String> {
}
