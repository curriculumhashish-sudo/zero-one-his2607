package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.equipment.TerConceptDO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentCategoryVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 * 编码概念 Mapper 接口
 * </p>
 *
 * @author xiaohan
 * @since 2026-07-27
 */
@Mapper
public interface TerConceptMapper extends BaseMapper<TerConceptDO> {

    /**
     * 根据name查询所属id后查询获取选择器材种类列表
     * @param equipmentCategoryCode 种类代码名称
     * @return 器材分类列表集合
     */
    @Select("SELECT tc.display AS label, tc.code AS `value` " +
            "FROM ter_concept tc JOIN ter_code_system tcs ON tc.code_system_id = tcs.id " +
            "WHERE tcs.name = #{equipmentCategoryCode}")
    List<EquipmentCategoryVO> queryEquipmentCategoryList(String equipmentCategoryCode);
}
