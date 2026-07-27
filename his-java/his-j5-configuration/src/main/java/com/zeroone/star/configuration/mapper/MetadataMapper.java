package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.configuration.entity.Metadata;
import org.apache.ibatis.annotations.Param;

/**
 * 元数据字段 Mapper。
 * <p>
 * 基于 MyBatis-Plus 的 {@link BaseMapper} 提供对 {@link Metadata} 实体的基础 CRUD 操作，
 * 同时扩展了针对关键词搜索优化的 UNION ALL 分页查询方法。
 * 操作的表为 {@code emr_metadata_field_def}。
 * </p>
 */
public interface MetadataMapper extends BaseMapper<Metadata> {

    /**
     * 使用 UNION ALL 策略进行关键词分页查询。
     * <p>
     * 将 field_key 和 label 的模糊匹配拆分为两个子查询，通过 UNION ALL 合并，
     * 避免 OR 条件导致的全表扫描。子查询间通过 NOT LIKE 去重。
     * 仅在用户输入关键词时调用此方法；无关键词时使用 MyBatis-Plus 标准分页。
     * </p>
     *
     * @param page        分页对象
     * @param isEnabled   启用状态筛选（必填，默认 1）
     * @param valueSource 取值来源筛选（可选）
     * @param keyword     搜索关键词
     * @return 分页结果
     */
    Page<Metadata> selectPageWithKeyword(Page<Metadata> page,
                                       @Param("isEnabled") Integer isEnabled,
                                       @Param("valueSource") String valueSource,
                                       @Param("keyword") String keyword);

    /**
     * 对应 {@link #selectPageWithKeyword} 的总记录数查询。
     *
     * @param isEnabled   启用状态筛选
     * @param valueSource 取值来源筛选（可选）
     * @param keyword     搜索关键词
     * @return UNION ALL 合并后的总记录数
     */
    Long selectCountWithKeyword(@Param("isEnabled") Integer isEnabled,
                                @Param("valueSource") String valueSource,
                                @Param("keyword") String keyword);
}