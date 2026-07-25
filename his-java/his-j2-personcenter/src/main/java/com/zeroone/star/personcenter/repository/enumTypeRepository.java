package com.zeroone.star.personcenter.repository;

import com.zeroone.star.personcenter.entity.EnumType;
import com.zeroone.star.personcenter.entity.EnumValue;
import org.springframework.data.mongodb.repository.Aggregation;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface enumTypeRepository extends MongoRepository<EnumType, String> {
    @Aggregation(pipeline = {
            "{$lookup: {from: 'EnumValue'," +
                    "localField: 'enum_type_key'," +
                    "foreignField: 'enum_key'," +
                    "as: 'enum_List'}," +
                    "{$unwind: {path: '$enum_List', preserveNullAndEmptyArrays: true}}," +
                    "{$project: {display: 1}}"
    })
    List<EnumValue> findByEnumKey(String enumKey);;
}
