package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.Frequency;
import com.zeroone.star.basedata.mapper.FrequencyMapper;
import com.zeroone.star.basedata.service.IFrequencyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencySaveDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyUpdateDTO;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

/**
 * <p>
 * 描述：频次管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class FrequencyServiceImpl extends ServiceImpl<FrequencyMapper, Frequency> implements IFrequencyService {

    @Resource
    private MsFrequencyMapper msFrequencyMapper;

    @Override
    public PageDTO<FrequencyVO> queryFrequencyPage(FrequencyPageQuery query) {
        // 构建分页对象
        Page<Frequency> page = new Page<>(query.getPageIndex(), query.getPageSize());
        // 构建查询条件
        LambdaQueryWrapper<Frequency> wrapper = new LambdaQueryWrapper<>();
        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(Frequency::getName, query.getKeyword())
                    .or()
                    .like(Frequency::getRateCode, query.getKeyword())
            );
        }
        // 按排序号升序排列
        wrapper.orderByAsc(Frequency::getSortNo);
        // 执行分页查询
        Page<Frequency> result = baseMapper.selectPage(page, wrapper);
        // 转换为 PageDTO<FrequencyVO>
        return PageDTO.create(result, FrequencyVO::new);
    }

    @Override
    public FrequencyVO queryFrequencyDetail(String id) {
        return msFrequencyMapper.entityToVo(baseMapper.selectById(id));
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public FrequencyVO addFrequency(FrequencySaveDTO dto) {
        return null;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public FrequencyVO modifyFrequency(FrequencyUpdateDTO dto) {
        return null;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void removeFrequency(String id) {
    }
}
