package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.MedicalTemplate;
import org.apache.ibatis.annotations.Param;

import java.time.LocalDateTime;

/**
 * 病历模板数据访问层。
 */
public interface MedicalTemplateMapper extends BaseMapper<MedicalTemplate> {

    /**
     * 统计编码占用数量，包含软删除记录，以匹配数据库唯一索引行为。
     */
    long countByCode(@Param("code") String code, @Param("excludeId") String excludeId);

    /**
     * 写入完整软删除审计信息。
     */
    int softDeleteById(@Param("id") String id,
                       @Param("deletedAt") LocalDateTime deletedAt,
                       @Param("deletedBy") String deletedBy);
}
