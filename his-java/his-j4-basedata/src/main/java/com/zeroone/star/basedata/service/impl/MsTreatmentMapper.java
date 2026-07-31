package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionChildDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentSelectDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentUpdateDTO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;
import com.zeroone.star.project.vo.j4.treatment.WkfActivityDefinitionChildVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.Mappings;
import org.mapstruct.Named;

import java.util.List;

@Mapper(componentModel = "spring")
public interface MsTreatmentMapper {

    @Mappings({
            @Mapping(source = "typeEnum", target = "typeEnum", qualifiedByName = "stringToInteger")
    })
    TreatmentSelectVO toSelectVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentSelectVO> toSelectVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    TreatmentSelectDTO toSelectDTO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentSelectDTO> toSelectDTOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(target = "price", ignore = true)
    })
    TreatmentDTO toDTO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentDTO> toDTOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(target = "price", ignore = true),
            @Mapping(source = "typeEnum", target = "typeEnum", qualifiedByName = "stringToInteger"),
            @Mapping(source = "statusEnum", target = "statusEnum", qualifiedByName = "statusStringToInteger")
    })
    TreatmentListVO toListVO(WkfActivityDefinitionDO wkfActivityDefinitionDO);

    List<TreatmentListVO> toListVOList(List<WkfActivityDefinitionDO> wkfActivityDefinitionDOS);

    @Mappings({
            @Mapping(source = "isDeleted", target = "deleteFlag"),
            @Mapping(source = "typeEnum", target = "typeEnum", qualifiedByName = "stringToInteger"),
            @Mapping(source = "statusEnum", target = "statusEnum", qualifiedByName = "statusStringToInteger"),
            @Mapping(target = "ybType", ignore = true),
            @Mapping(target = "itemTypeCode", ignore = true),
            @Mapping(target = "purchasePrice", ignore = true),
            @Mapping(target = "retailPrice", ignore = true),
            @Mapping(target = "maximumRetailPrice", ignore = true),
            @Mapping(target = "children", ignore = true)
    })
    TreatmentDetailVO toDetailVO(WkfActivityDefinitionDO entity);

    WkfActivityDefinitionChildVO toChildVO(WkfActivityDefinitionChildDO childDO);

    List<WkfActivityDefinitionChildVO> toChildVOList(List<WkfActivityDefinitionChildDO> childDOS);

    @Mappings({
            @Mapping(target = "id", ignore = true),
            @Mapping(target = "tenantId", ignore = true),
            @Mapping(target = "createdAt", ignore = true),
            @Mapping(target = "updatedAt", ignore = true),
            @Mapping(target = "createdBy", ignore = true),
            @Mapping(target = "updatedBy", ignore = true),
            @Mapping(target = "isDeleted", ignore = true),
            @Mapping(target = "deletedAt", ignore = true),
            @Mapping(target = "deletedBy", ignore = true),
            @Mapping(target = "version", ignore = true),
            @Mapping(target = "childrenJson", ignore = true),
            @Mapping(target = "typeEnum", ignore = true)
    })
    WkfActivityDefinitionDO addDtoToEntity(TreatmentAddDTO dto);

    @Mappings({
            @Mapping(target = "tenantId", ignore = true),
            @Mapping(target = "createdAt", ignore = true),
            @Mapping(target = "updatedAt", ignore = true),
            @Mapping(target = "createdBy", ignore = true),
            @Mapping(target = "updatedBy", ignore = true),
            @Mapping(target = "isDeleted", ignore = true),
            @Mapping(target = "deletedAt", ignore = true),
            @Mapping(target = "deletedBy", ignore = true),
            @Mapping(target = "version", ignore = true),
            @Mapping(target = "childrenJson", ignore = true),
            @Mapping(target = "typeEnum", ignore = true)
    })
    WkfActivityDefinitionDO editDtoToEntity(TreatmentEditDTO dto);

    @Mappings({
            @Mapping(target = "tenantId", ignore = true),
            @Mapping(target = "createdAt", ignore = true),
            @Mapping(target = "updatedAt", ignore = true),
            @Mapping(target = "createdBy", ignore = true),
            @Mapping(target = "updatedBy", ignore = true),
            @Mapping(target = "isDeleted", ignore = true),
            @Mapping(target = "deletedAt", ignore = true),
            @Mapping(target = "deletedBy", ignore = true),
            @Mapping(target = "version", ignore = true),
            @Mapping(target = "childrenJson", ignore = true),
            @Mapping(target = "typeEnum", ignore = true)
    })
    WkfActivityDefinitionDO updateDtoToEntity(TreatmentUpdateDTO dto);

    /**
     * 将数字字符串安全转换为Integer，用于typeEnum等枚举字段从数据库String到前端Integer的转换
     */
    @Named("stringToInteger")
    default Integer stringToInteger(String value) {
        if (value == null || value.isBlank()) {
            return null;
        }
        try {
            return Integer.valueOf(value);
        } catch (NumberFormatException ex) {
            return null;
        }
    }

    /**
     * 将Integer转换为字符串，用于前端传Integer到数据库String字段的转换
     */
    @Named("integerToString")
    default String integerToString(Integer value) {
        if (value == null) {
            return null;
        }
        return String.valueOf(value);
    }

    /**
     * 将状态字符串转换为对应的sortOrder数字（对齐参考项目PublicationStatus枚举）
     * DRAFT=1(草稿), ACTIVE=2(已启用), INACTIVE=3(已停用), DISCARDED=4(已废弃)
     */
    @Named("statusStringToInteger")
    default Integer statusStringToInteger(String statusCode) {
        if (statusCode == null || statusCode.isBlank()) {
            return null;
        }
        switch (statusCode) {
            case "DRAFT":
                return 1;
            case "ACTIVE":
                return 2;
            case "INACTIVE":
                return 3;
            case "DISCARDED":
                return 4;
            default:
                return null;
        }
    }

    /**
     * 将状态数字转换为对应的字符串code（反向转换）
     * 1=DRAFT, 2=ACTIVE, 3=INACTIVE, 4=DISCARDED
     */
    @Named("statusIntegerToString")
    default String statusIntegerToString(Integer statusValue) {
        if (statusValue == null) {
            return null;
        }
        switch (statusValue) {
            case 1:
                return "DRAFT";
            case 2:
                return "ACTIVE";
            case 3:
                return "INACTIVE";
            case 4:
                return "DISCARDED";
            default:
                return null;
        }
    }
}
