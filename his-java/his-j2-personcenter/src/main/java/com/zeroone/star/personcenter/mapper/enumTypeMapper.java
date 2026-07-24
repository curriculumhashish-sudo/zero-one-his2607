package com.zeroone.star.personcenter.mapper;

import com.zeroone.star.personcenter.entity.EnumType;
import com.zeroone.star.project.query.j2.sys.EnumListQuery;
import org.springframework.data.mongodb.repository.MongoRepository;

import java.util.List;

public interface enumTypeMapper extends MongoRepository<EnumType, String> {

    String getEnumNameListByQuery(EnumListQuery query);
}
