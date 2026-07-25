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

    List<SupplierNameVO> selectSupplierNames(@Param("statusEnum") String statusEnum, @Param("keyword") String keyword);

    Page<SupplierListVO> selectSupplierPage(Page<SupplierListVO> page, @Param("query") SupplierQuery query);

    SupplierDetailVO selectSupplierDetail(@Param("id") String id);
}
