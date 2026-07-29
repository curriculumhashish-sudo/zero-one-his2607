package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.basedata.entity.diagnosis.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.entity.diagnosis.TerConceptDO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;
import org.mapstruct.ReportingPolicy;
import java.util.List;

/**
 * <p>
 * 描述：诊断管理对应MapStruct映射接口
 * </p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper(componentModel = "spring", unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsDiagnosisMapper {

    /**
     * 将DTO映射为数据库实体
     * @param dto 诊断定义DTO
     * @return 诊断定义实体
     */
    ClnConditionDefinitionDO dtoToEntity(DiagnosisDefinitionDTO dto);

    /**
     * 将数据库实体映射为详情/列表展示VO
     * @param entity 诊断定义实体
     * @return 诊断定义详情VO
     */
    DiagnosisDefinitionDetailVO entityToDetailVo(ClnConditionDefinitionDO entity);

    /**
     * 将数据库实体列表映射为详情/列表展示VO列表
     * @param entities 诊断定义实体列表
     * @return 诊断定义详情VO列表
     */
    List<DiagnosisDefinitionDetailVO> entitiesToDetailVos(List<ClnConditionDefinitionDO> entities);

    @Mapping(source = "code", target = "sourceEnum")
    @Mapping(source = "display", target = "sourceName")
    DiagnosisOptionVO conceptToOptionVo(TerConceptDO entity);

    /**
     * 将数据库实体映射为诊断管理列表项。
     */
    DiagnosisListVO entityToListVo(ClnConditionDefinitionDO entity);

    /**
     * 将数据库实体映射为诊断详情。
     */
    DiagnosisDetailVO entityToDiagnosisDetailVo(ClnConditionDefinitionDO entity);

    /**
     * 数据库使用 Boolean，现有 VO 使用 0/1 表示标记。
     */
    default Integer booleanToInteger(Boolean value) {
        return value == null ? null : (value ? 1 : 0);
    }
}
