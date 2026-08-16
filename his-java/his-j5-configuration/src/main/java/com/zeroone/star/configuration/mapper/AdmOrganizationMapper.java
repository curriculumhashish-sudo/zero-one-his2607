package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.AdmOrganizationDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 * 描述：机构/科室Mapper（执行地点）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Mapper
public interface AdmOrganizationMapper extends BaseMapper<AdmOrganizationDO> {

    /**
     * 查询启用的科室列表（类型为DEPT）
     */
    @Select("SELECT id, org_code, org_name, org_type, parent_id, sort_order, is_enabled " +
            "FROM adm_organization WHERE org_type = 'DEPT' AND is_deleted = 0 AND is_enabled = 1 " +
            "ORDER BY sort_order ASC, org_name ASC")
    List<AdmOrganizationDO> selectEnabledDeptList();

    /**
     * 根据ID查询机构/科室名称
     */
    @Select("SELECT org_name FROM adm_organization WHERE id = #{id} AND is_deleted = 0")
    String selectNameById(@Param("id") String id);
}
