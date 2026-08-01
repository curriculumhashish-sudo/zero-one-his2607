package com.zeroone.star.setroster.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.setroster.entity.BkgReleasePolicy;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Update;

import java.time.LocalDateTime;

/**
 * 放号策略数据访问接口。
 */
@Mapper
public interface BkgReleasePolicyMapper extends BaseMapper<BkgReleasePolicy> {

    /**
     * 原子完成逻辑删除、审计字段写入和乐观锁版本递增。
     */
    @Update("""
            UPDATE bkg_release_policy
            SET is_deleted = 1,
                deleted_at = #{deletedAt},
                deleted_by = #{deletedBy},
                updated_at = #{deletedAt},
                updated_by = #{deletedBy},
                version = version + 1
            WHERE id = #{id}
              AND is_deleted = 0
              AND version = #{version}
            """)
    int logicallyDelete(
            @Param("id") String id,
            @Param("version") Integer version,
            @Param("deletedAt") LocalDateTime deletedAt,
            @Param("deletedBy") String deletedBy);
}
