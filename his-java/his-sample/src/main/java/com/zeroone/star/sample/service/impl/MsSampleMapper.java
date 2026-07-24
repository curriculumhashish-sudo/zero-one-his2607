package com.zeroone.star.sample.service.impl;

import com.zeroone.star.project.dto.sample.SampleAddDTO;
import com.zeroone.star.project.dto.sample.SampleDTO;
import com.zeroone.star.sample.entity.Sample;
import org.mapstruct.Mapper;
import org.mapstruct.ReportingPolicy;

/**
 * Sample对应MapStruct映射接口
 */
@Mapper(componentModel = "spring",unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsSampleMapper {
    /**
     * 映射Sample到SampleDTO
     * @param entity
     * @return
     */
    SampleDTO sampleToSampleDto(Sample entity);

    /**
     * 映射SampleDTO到Sample
     * @param sampleDTO
     * @return
     */
    Sample sampleDtoToSample(SampleDTO sampleDTO);

    /**
     * 映射SampleAddDto到Sample
     * @param sampleAddDTO
     * @return
     */
    Sample sampleAddDtoToSample(SampleAddDTO sampleAddDTO);
}
