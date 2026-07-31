package com.zeroone.star.report.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.project.vo.j6.report.ExpirationWarningVO;
import com.zeroone.star.report.entity.WkfInventoryItem;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

/**
 * <p>
 * 描述：库存明细 Mapper 数据操作层
 * </p>
 * @author j6
 * @version 1.0.0
 */
@Mapper
public interface WkfInventoryItemMapper extends BaseMapper<WkfInventoryItem> {

    /**
     * 查询剩余有效期在指定预警天数内的物资列表
     *
     * @param expireWithinDays 剩余有效天数阈值（如 90 天）
     * @return 有效期预警数据点列表
     */
    @Select("SELECT " +
            "i.name AS `name`, " +
            "i.lot_number AS `lotNumber`, " +
            "DATE_FORMAT(i.expiration_date, '%Y-%m-%d') AS `expirationDate`, " +
            "DATEDIFF(i.expiration_date, NOW()) AS `remainingDays`, " +
            "i.quantity AS `quantity`, " +
            "COALESCE(l.name, '综合库房') AS `warehouseName`, " +
            "COALESCE(s.name, '通用供应商') AS `supplierName` " +
            "FROM wkf_inventory_item i " +
            "LEFT JOIN adm_location l ON i.location_id = l.id " +
            "LEFT JOIN cus_supplier s ON i.supplier_id = s.id " +
            "WHERE i.is_deleted = 0 " +
            "AND i.expiration_date IS NOT NULL " +
            "AND DATEDIFF(i.expiration_date, NOW()) <= #{expireWithinDays} " +
            "ORDER BY `remainingDays` ASC")
    List<ExpirationWarningVO> selectExpirationWarningList(@Param("expireWithinDays") Integer expireWithinDays);
}
