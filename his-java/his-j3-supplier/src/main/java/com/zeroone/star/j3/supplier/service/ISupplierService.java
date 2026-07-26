package com.zeroone.star.j3.supplier.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.j3.supplier.entity.Supplier;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierBatchDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierFullDTO;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;

import java.util.List;

/**
 * <p>
 * 供应商表 服务类
 * </p>
 *
 * @author 业之
 * @since 2026-07-20
 */
public interface ISupplierService extends IService<Supplier> {

    List<SupplierNameVO> querySupplierNames(String statusEnum, String keyword);

    PageDTO<SupplierListVO> querySupplierList(SupplierQuery query);

    SupplierDetailVO querySupplierDetail(String id);

    String addSupplier(SupplierFullDTO dto);

    String updateSupplier(String id, SupplierFullDTO dto);

    String deleteSupplier(String id);

    String batchUpdateSuppliers(SupplierBatchDTO batchDTO);
}
