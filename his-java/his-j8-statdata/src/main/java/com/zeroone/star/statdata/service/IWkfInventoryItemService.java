package com.zeroone.star.statdata.service;

import com.zeroone.star.project.dto.j8.statdata.ExpireWarningDTO;
import com.zeroone.star.statdata.entity.WkfInventoryItem;
import com.baomidou.mybatisplus.extension.service.IService;

import java.util.List;
/**
 * <p>
 * 库存有效期服务接口
 * </p>
 *
 * @author highxin
 * @since 2026-07-24
 */
public interface IWkfInventoryItemService extends IService<WkfInventoryItem> {
    /**
     * 获取过期预警数据
     * @param goodsType 商品类型
     * @return 过期预警数据
     */
    List<ExpireWarningDTO> getExpireWarningData(String goodsType);
}