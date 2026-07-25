package com.zeroone.star.personcenter.repository;

import com.zeroone.star.personcenter.entity.EnumValue;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDTO;
import com.zeroone.star.project.query.j2.sys.EnumQuery;
import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.data.mongodb.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.List;


@Repository
public interface enumRepository extends MongoRepository<EnumValue, String> {
//    @Query("{'display': ?1, 'enumKey': ?1}")
//    List<EnumDTO> findByEnumQuery(EnumQuery query);
}

