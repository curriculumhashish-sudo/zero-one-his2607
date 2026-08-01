package com.zeroone.star.j3.supplier.utils;

import com.zeroone.star.j3.supplier.entity.Supplier;
import com.zeroone.star.project.dto.j3.supplier.SupplierAddDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierUpdateDTO;
import org.mapstruct.Mapper;
import org.mapstruct.ReportingPolicy;

/**
 * 描述：对于supplier模块不同的数据模型的转换
 *
 * @author： yezhi
 * @version： 1.0
 * @date： 2026/7/20 10:07
 */
@Mapper(componentModel = "spring" ,unmappedTargetPolicy = ReportingPolicy.IGNORE)
public interface MsSupplierMapper {
    /**
     * SupplierFullDTO 转换为 Supplier
     * @param supplierDTO
     * @return
     */
    Supplier SupplierAddDTO2Supplier(SupplierAddDTO supplierDTO);


    /**
     * SupplierFullVO 转换为 Supplier
     * @param detailVO
     * @return
     */
    Supplier SupplierUpdateVO2Supplier(SupplierUpdateDTO detailVO);
}
