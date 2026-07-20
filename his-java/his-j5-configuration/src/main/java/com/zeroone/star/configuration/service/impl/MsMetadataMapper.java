package com.zeroone.star.configuration.service.impl;

import com.zeroone.star.project.dto.j5.configuration.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataListDTO;
import com.zeroone.star.configuration.entity.Metadata;
import org.mapstruct.Mapper;
import org.mapstruct.ReportingPolicy;

/**
 * 元数据字段实体与 DTO 之间的 MapStruct 转换器。
 * <p>
 * 基于 MapStruct 在编译期自动生成 {@code Metadata} 实体到
 * {@link MetadataListDTO} 和 {@link MetadataDetailDTO} 的映射实现，
 * 未映射的字段将被忽略。生成实现以 Spring Bean 方式注入容器。
 * </p>
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsMetadataMapper {

    MetadataListDTO metadataFieldToListDTO(Metadata entity);

    MetadataDetailDTO metadataFieldToDetailDTO(Metadata entity);
}