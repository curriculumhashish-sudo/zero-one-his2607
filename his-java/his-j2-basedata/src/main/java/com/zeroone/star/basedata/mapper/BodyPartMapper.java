package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.BodyPart;
import com.zeroone.star.project.doo.BodyPartDO;
import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartTreeVO;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import java.util.List;

/**
 * 部位数据访问接口。
 */
public interface BodyPartMapper extends BaseMapper<BodyPart> {

    List<BodyPartTreeVO> selectTree();

    List<BodyPartListVO> selectList(@Param("query") BodyPartPageQuery query,
                                    @Param("offset") int offset,
                                    @Param("limit") int limit);

    Long selectListCount(@Param("query") BodyPartPageQuery query);

    // DO新增自定义insert
    @Insert("""
            INSERT INTO dia_body_structure
            (id,bus_no,name,parent_id,tenant_id,status_enum,is_deleted,version,created_at,updated_at,created_by,updated_by)
            VALUES(#{id},#{busNo},#{name},#{parentId},#{tenantId},#{statusEnum},#{isDeleted},#{version},#{createdAt},#{updatedAt},#{createdBy},#{updatedBy})
            """)
    int insert(BodyPartDO bodyPartDO);

    @Select("""
            SELECT CASE WHEN EXISTS (
                SELECT 1 FROM cln_condition
                WHERE body_structure_id = #{bodyStructureId} AND is_deleted = 0
            ) OR EXISTS (
                SELECT 1 FROM cln_procedure
                WHERE body_structure_id = #{bodyStructureId} AND is_deleted = 0
            ) THEN 1 ELSE 0 END
            """)
    boolean existsClinicalReference(@Param("bodyStructureId") String bodyStructureId);
}