package com.zeroone.star.configuration.mapper;

import com.zeroone.star.project.dto.j5.configuration.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataListDTO;
import com.zeroone.star.configuration.entity.Metadata;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;

/**
 * 元数据字段实体与 DTO 之间的 MapStruct 转换器。
 * <p>
 * 基于 MapStruct 在编译期自动生成 {@code Metadata} 实体到
 * {@link MetadataListDTO} 和 {@link MetadataDetailDTO} 的映射实现。
 * 实体字段名与 DTO 字段名存在差异（实体匹配数据库列名，DTO 匹配前端接口规范），
 * 通过 {@link Mapping} 注解显式指定映射关系。
 * </p>
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsMetadataMapper {

    /**
     * 将实体转换为列表项 DTO。
     * <p>列表仅包含简要信息：唯一标识、字段标识、显示名、编辑类型、取值来源。</p>
     */
    @Mapping(source = "label", target = "displayName")
    @Mapping(source = "editableDefault", target = "editType")
    MetadataListDTO metadataFieldToListDTO(Metadata entity);

    /**
     * 将实体转换为详情 DTO。
     * <p>详情包含完整的字段元信息。</p>
     */
    @Mapping(source = "label", target = "displayName")
    @Mapping(source = "dataType", target = "fieldType")
    @Mapping(source = "editableDefault", target = "editType")
    @Mapping(source = "isEnabled", target = "status")
    @Mapping(source = "remark", target = "description")
    @Mapping(target = "length", ignore = true)
    @Mapping(target = "precision", ignore = true)
    @Mapping(target = "nullable", ignore = true)
    MetadataDetailDTO metadataFieldToDetailDTO(Metadata entity);
}
