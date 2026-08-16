package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.MedicalTemplateDO;
import org.apache.ibatis.annotations.Param;

import java.time.LocalDateTime;

/**
 * 病历模板数据访问层。
 */
public interface MedicalTemplateMapper extends BaseMapper<MedicalTemplateDO> {

    /**
     * 判断编码是否已被占用，包含软删除记录，以匹配数据库唯一索引行为。
     */
    boolean existsByCode(@Param("code") String code, @Param("excludeId") String excludeId);

    /**
     * 查询修改和作用域校验所需的最小字段集合。
     */
    MedicalTemplateDO selectScopeById(@Param("id") String id);

    /**
     * 按请求中提供的字段更新模板。
     */
    int updateSelectiveById(@Param("template") MedicalTemplateDO template,
                            @Param("codeChanged") boolean codeChanged,
                            @Param("scopeChanged") boolean scopeChanged);

    /**
     * 写入完整软删除审计信息。
     */
    int softDeleteById(@Param("id") String id,
                       @Param("deletedAt") LocalDateTime deletedAt,
                       @Param("deletedBy") String deletedBy);
}
