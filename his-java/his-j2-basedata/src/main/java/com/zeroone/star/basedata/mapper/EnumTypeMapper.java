package com.zeroone.star.basedata.mapper;

import com.zeroone.star.basedata.entity.EnumType;
import org.springframework.data.mongodb.repository.MongoRepository;

public interface EnumTypeMapper extends MongoRepository<EnumType, String> {
}
