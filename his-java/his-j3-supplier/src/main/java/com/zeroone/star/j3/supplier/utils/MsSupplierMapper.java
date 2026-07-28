package com.zeroone.star.j3.supplier.utils;

import ch.qos.logback.core.model.ComponentModel;
import com.zeroone.star.j3.supplier.entity.Supplier;
import com.zeroone.star.project.dto.j3.supplier.SupplierFullDTO;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
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
    Supplier SupplierFullDTO2Supplier(SupplierFullDTO supplierDTO);


    /**
     * SupplierFullVO 转换为 Supplier
     * @param detailVO
     * @return
     */
    Supplier SupplierDetailVO2Supplier(SupplierDetailVO detailVO);
}
