package com.zeroone.star.sysmgr.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.doo.WhaleSettingsDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * <p>
 * 描述：系统配置项 Mapper 接口
 * </p>
 *
 * @author L
 * @version 1.0.0
 */
@Mapper
public interface SettingsMapper extends BaseMapper<WhaleSettingsDO> {

    /**
     * 分页查询配置项
     */
    @Select("SELECT id, name, value, provider_name, provider_key, description, " +
            "created_at, updated_at, created_by, updated_by " +
            "FROM whale_settings " +
            "WHERE tenant_id = #{tenantId} AND is_deleted = 0 " +
            "ORDER BY created_at DESC")
    Page<WhaleSettingsDO> queryAll(@Param("page") Page<WhaleSettingsDO> page,
                                   @Param("tenantId") String tenantId);

    /**
     * 按条件分页查询配置项
     */
    @Select("<script>" +
            "SELECT id, name, value, provider_name, provider_key, description, " +
            "created_at, updated_at, created_by, updated_by " +
            "FROM whale_settings " +
            "WHERE tenant_id = #{tenantId} AND is_deleted = 0" +
            "<if test='query.provider_name != null and query.provider_name != \"\"'>" +
            " AND provider_name = #{query.provider_name}" +
            "</if>" +
            "<if test='query.provider_key != null and query.provider_key != \"\"'>" +
            " AND provider_key = #{query.provider_key}" +
            "</if>" +
            "<if test='query.value != null and query.value != \"\"'>" +
            " AND value = #{query.value}" +
            "</if>" +
            " ORDER BY created_at DESC" +
            "</script>")
    Page<WhaleSettingsDO> queryByCondition(@Param("page") Page<WhaleSettingsDO> page,
                                           @Param("query") com.zeroone.star.project.query.j2.sysmgr.settings.SettingsQuery query,
                                           @Param("tenantId") String tenantId);

    /**
     * 校验配置项是否已存在
     */
    @Select("SELECT COUNT(1) FROM whale_settings " +
            "WHERE name = #{name} AND tenant_id = #{tenantId} AND is_deleted = 0")
    int countByNameAndTenant(@Param("name") String name,
                             @Param("tenantId") String tenantId);
}
