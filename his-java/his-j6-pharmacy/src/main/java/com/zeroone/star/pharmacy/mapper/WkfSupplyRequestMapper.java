package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.project.query.j6.pharmacy.stocktake.PharmacyInventoryQuery;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.PharmacyInventoryVO;
import org.apache.ibatis.annotations.Param;

/**
 * <p>
 * 工作流 supply request Mapper 接口
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface WkfSupplyRequestMapper extends BaseMapper<WkfSupplyRequest> {



}
