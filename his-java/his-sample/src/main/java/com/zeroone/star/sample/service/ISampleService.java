package com.zeroone.star.sample.service;

import cn.hutool.db.Page;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.sample.SampleDTO;
import com.zeroone.star.project.query.sample.SampleQuery;
import com.zeroone.star.sample.entity.Sample;
import com.baomidou.mybatisplus.extension.service.IService;

/**
 * <p>
 * 演示示例表 服务类
 * </p>
 *
 * @author 告别
 * @since 2026-07-12
 */
public interface ISampleService extends IService<Sample> {
    /**
     * 获取所有数据
     * @param query 查询参数
     * @return 所有数据
     */
    PageDTO<SampleDTO> listAll(SampleQuery query);

    SampleDTO getById(String id);
}
