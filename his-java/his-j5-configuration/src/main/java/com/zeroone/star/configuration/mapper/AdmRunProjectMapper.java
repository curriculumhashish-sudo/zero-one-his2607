package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.AdmRunProjectDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 * 描述：执行项目Mapper（避免select *，指定字段）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Mapper
public interface AdmRunProjectMapper extends BaseMapper<AdmRunProjectDO> {

    /**
     * 根据执行科室ID查询执行项目列表
     */
    @Select("SELECT id, item_code, item_name, exec_dept_id, exec_dept_name, sort_order, is_enabled " +
            "FROM adm_run_project WHERE exec_dept_id = #{deptId} AND is_deleted = 0 " +
            "ORDER BY sort_order ASC, created_at DESC")
    List<AdmRunProjectDO> selectByDeptId(@Param("deptId") String deptId);

    /**
     * 根据项目编码查询（去重校验）
     */
    @Select("SELECT COUNT(1) FROM adm_run_project WHERE item_code = #{itemCode} AND is_deleted = 0")
    Long countByItemCode(@Param("itemCode") String itemCode);
}
