package com.zeroone.star.pharmacy.service;

import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.j6.pharmacy.inventory.InventoryStatusDTO;

/**
 * <p>
 * 工作流 inventory item 服务类
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface IWkfInventoryItemService extends IService<WkfInventoryItem> {

    Integer resumeInventoryItems(InventoryStatusDTO ids);
}
