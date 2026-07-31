package com.zeroone.star.sample.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.sample.SampleDTO;
import com.zeroone.star.project.query.sample.SampleQuery;
import com.zeroone.star.sample.entity.Sample;
import com.zeroone.star.sample.mapper.SampleMapper;
import com.zeroone.star.sample.service.ISampleService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

/**
 * <p>
 * 演示示例表 服务实现类
 * </p>
 *
 * @author 告别
 * @since 2026-07-12
 */
@Service
public class SampleServiceImpl extends ServiceImpl<SampleMapper, Sample> implements ISampleService {

    @Resource
    MsSampleMapper msSampleMapper;

    @Override
    public PageDTO<SampleDTO> listAll(SampleQuery query) {
        //构建一个分页对象
        Page<Sample> page = new Page<>(query.getPageIndex(),query.getPageSize());
        //构建查询条件
        QueryWrapper<Sample> queryWrapper = new QueryWrapper<>();
        queryWrapper.like(StringUtils.hasText(query.getName()),"name",query.getName());
        queryWrapper.orderBy(true,false,"IFNULL('update_time','create_time')");
        queryWrapper.orderBy(true,false,"id");
        //执行查询
        Page<Sample> samplePage = baseMapper.selectPage(page,queryWrapper);
        return PageDTO.create(samplePage,msSampleMapper::sampleToSampleDto);
    }

    @Override
    public SampleDTO getById(String id) {
        Sample sample = baseMapper.selectById(id);
        if(sample != null){
            return msSampleMapper.sampleToSampleDto(sample);
        }
        return null;
    }
}
