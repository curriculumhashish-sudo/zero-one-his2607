package com.zeroone.star.personcenter.mapper;

import com.zeroone.star.personcenter.entity.EnumType;
import org.springframework.data.mongodb.repository.MongoRepository;


public interface enumTypeMapper extends MongoRepository<EnumType, String> {

}
