package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.BodyPart;
import org.apache.ibatis.annotations.Select;

/**
 * 部位数据访问接口。
 */
public interface BodyPartMapper extends BaseMapper<BodyPart> {

    /**
     * 判断部位是否仍被临床病情或临床处置使用。
     *
     * @param bodyStructureId 部位标识
     * @return 存在未删除的业务引用时为 true
     */
    @Select("""
            SELECT CASE WHEN EXISTS (
                SELECT 1 FROM cln_condition
                WHERE body_structure_id = #{bodyStructureId} AND is_deleted = 0
            ) OR EXISTS (
                SELECT 1 FROM cln_procedure
                WHERE body_structure_id = #{bodyStructureId} AND is_deleted = 0
            ) THEN 1 ELSE 0 END
            """)
    boolean existsClinicalReference(String bodyStructureId);
}
