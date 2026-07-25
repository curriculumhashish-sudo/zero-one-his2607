package com.zeroone.star.personcenter.repository;

import com.zeroone.star.personcenter.entity.EnumValue;
import com.zeroone.star.project.dto.PageDTO;
import org.springframework.data.mongodb.repository.Aggregation;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface enumRepository extends MongoRepository<EnumValue, String> {

}

