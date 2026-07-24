package com.zeroone.star.personcenter.mapper;

import com.zeroone.star.personcenter.entity.EnumValue;
import org.springframework.data.mongodb.repository.MongoRepository;

public interface enumMapper extends MongoRepository<EnumValue, String> {
}
