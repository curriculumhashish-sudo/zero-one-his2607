package com.zeroone.star.j3.supplier.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;

import com.zeroone.star.j3.supplier.entity.Supplier;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 供应商表 Mapper 接口
 * </p>
 *
 * @author 业之
 * @since 2026-07-20
 */
public interface SupplierMapper extends BaseMapper<Supplier> {

    List<SupplierNameVO> selectSupplierNames(@Param("statusEnum") String statusEnum, @Param("keyword") String keyword, @Param("tenantId") String tenantId);

    Page<SupplierListVO> selectSupplierPage(Page<SupplierListVO> page, @Param("query") SupplierQuery query, @Param("tenantId") String tenantId);

    SupplierDetailVO selectSupplierDetail(@Param("id") String id, @Param("tenantId") String tenantId);

    String selectCreditCodeById(@Param("id") String id, @Param("tenantId") String tenantId);

    /**
     * 悲观锁方式查询当前最大ID，用于并发安全的ID自增生成
     * FOR UPDATE 锁定行，防止并发请求读到同一个MAX(id)
     * @return 当前最大ID，表为空时返回NULL
     */
    String selectMaxIdForUpdate();
}
