package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.DictionaryConceptDO;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

/**
 * 字典项数据访问层。
 */
public interface DictionaryMapper extends BaseMapper<DictionaryConceptDO> {

    @Select("""
            SELECT id
            FROM ter_code_system
            WHERE id = #{codeSystemId}
              AND is_deleted = 0
            FOR UPDATE
            """)
    String lockCodeSystem(@Param("codeSystemId") String codeSystemId);

    @Select("""
            SELECT COUNT(1)
            FROM ter_concept
            WHERE code_system_id = #{codeSystemId}
              AND code = #{code}
              AND is_deleted = 0
              AND (#{excludeId} IS NULL OR id <> #{excludeId})
            """)
    int countDuplicateCode(@Param("codeSystemId") String codeSystemId,
                           @Param("code") String code,
                           @Param("excludeId") String excludeId);

    @Select("""
            SELECT COUNT(1)
            FROM ter_concept
            WHERE id = #{id}
              AND is_deleted = 0
            """)
    int countConcept(@Param("id") String id);

    @Select("""
            SELECT COALESCE(MAX(sort_order), -1)
            FROM ter_concept
            WHERE code_system_id = #{codeSystemId}
              AND is_deleted = 0
            """)
    Long selectMaxSortOrder(@Param("codeSystemId") String codeSystemId);

    @Update("""
            UPDATE ter_concept
            SET code_system_id = #{concept.codeSystemId},
                code = #{concept.code},
                display = #{concept.display},
                definition = #{concept.definition},
                sort_order = COALESCE(#{concept.sortOrder}, sort_order),
                updated_at = #{concept.updatedAt},
                updated_by = #{concept.updatedBy}
            WHERE id = #{concept.id}
              AND is_deleted = 0
            """)
    int updateConcept(@Param("concept") DictionaryConceptDO concept);

    @Update("""
            UPDATE ter_concept
            SET is_deleted = 1,
                deleted_at = CURRENT_TIMESTAMP(6),
                deleted_by = #{operatorId},
                updated_at = CURRENT_TIMESTAMP(6),
                updated_by = #{operatorId}
            WHERE id = #{id}
              AND is_deleted = 0
            """)
    int logicallyDelete(@Param("id") String id, @Param("operatorId") String operatorId);
}
