package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 工作流 supply request Mapper 接口
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface WkfSupplyRequestMapper extends BaseMapper<WkfSupplyRequest> {

    /**
     * 查询指定业务单号下未删除的进货单行。
     * 仅查询写操作校验所需字段，避免 select *。
     *
     * @param busNo 业务单号
     * @return 单据行
     */
    @Select("""
            SELECT id, bus_no, status_enum, version, is_deleted
            FROM wkf_supply_request
            WHERE bus_no = #{busNo}
              AND is_deleted = 0
            """)
    List<WkfSupplyRequest> selectActiveRowsByBusNo(@Param("busNo") String busNo);

    /**
     * 统计指定业务单号下未删除的进货单行数。
     *
     * @param busNo 业务单号
     * @return 行数
     */
    @Select("""
            SELECT COUNT(1)
            FROM wkf_supply_request
            WHERE bus_no = #{busNo}
              AND is_deleted = 0
            """)
    long countActiveRowsByBusNo(@Param("busNo") String busNo);

    /**
     * 软删除指定业务单号下未删除的进货单行。
     *
     * @param busNo 业务单号
     * @param deletedAt 删除时间
     * @param deletedBy 删除人
     * @param updatedAt 更新时间
     * @param updatedBy 更新人
     * @return 影响行数
     */
    @Update("""
            UPDATE wkf_supply_request
            SET is_deleted = 1,
                deleted_at = #{deletedAt},
                deleted_by = #{deletedBy},
                updated_at = #{updatedAt},
                updated_by = #{updatedBy}
            WHERE bus_no = #{busNo}
              AND is_deleted = 0
            """)
    int updateActiveRowsDeletedByBusNo(@Param("busNo") String busNo,
                                       @Param("deletedAt") LocalDateTime deletedAt,
                                       @Param("deletedBy") String deletedBy,
                                       @Param("updatedAt") LocalDateTime updatedAt,
                                       @Param("updatedBy") String updatedBy);
}
