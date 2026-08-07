package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.AdmRunProjectDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 * 描述：执行项目Mapper（映射adm_healthcare_service表）
 * </p>
 * @author zeroone
 * @version 1.0.0
 */
@Mapper
public interface AdmRunProjectMapper extends BaseMapper<AdmRunProjectDO> {

    /**
     * 根据执行地点ID查询执行项目列表
     */
    @Select("SELECT id, name, type_code, category_code, location_id, status_enum, active_flag, comment " +
            "FROM adm_healthcare_service WHERE location_id = #{locationId} AND is_deleted = 0 " +
            "ORDER BY created_at DESC")
    List<AdmRunProjectDO> selectByLocationId(@Param("locationId") String locationId);

    /**
     * 根据名称查询（去重校验）
     */
    @Select("SELECT COUNT(1) FROM adm_healthcare_service WHERE name = #{name} AND is_deleted = 0")
    Long countByName(@Param("name") String name);
}
