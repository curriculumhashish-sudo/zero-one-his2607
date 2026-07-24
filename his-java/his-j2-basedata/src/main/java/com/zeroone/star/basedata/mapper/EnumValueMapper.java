package com.zeroone.star.basedata.mapper;

import com.zeroone.star.basedata.entity.EnumValue;
import org.springframework.data.mongodb.repository.MongoRepository;

public interface EnumValueMapper extends MongoRepository<EnumValue, String> {
}
