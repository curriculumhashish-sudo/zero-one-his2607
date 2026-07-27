package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WarehouseInventoryDetailPageQuery;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.project.vo.j6.pharmacy.inventory.InventoryDetailVO;
import org.apache.ibatis.annotations.Param;

/**
 * <p>
 * 工作流 inventory item Mapper 接口
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface WkfInventoryItemMapper extends BaseMapper<WkfInventoryItem> {

    IPage<InventoryDetailVO> selectCurrentDetailPage(Page<InventoryDetailVO> page, @Param("query") WarehouseInventoryDetailPageQuery query);
}
