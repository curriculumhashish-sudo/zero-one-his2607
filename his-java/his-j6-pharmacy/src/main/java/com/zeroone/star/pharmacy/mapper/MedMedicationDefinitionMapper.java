package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.MedMedicationDefinition;

import java.util.List;

/**
 * <p>
 * 药品 medication 定义 Mapper 接口
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface MedMedicationDefinitionMapper extends BaseMapper<MedMedicationDefinition> {

    // 根据分类代码查询药品ID列表
    List<Long> selectIdsByCategoryCodes(List<String> medicationCategoryCodes);

    List<Long> selectIdsByKeyword(String kw);
}
