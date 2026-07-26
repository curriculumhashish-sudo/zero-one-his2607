package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.frequency.CusFrequencyDO;
import com.zeroone.star.basedata.mapper.FrequencyMapper;
import com.zeroone.star.basedata.service.IFrequencyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencySaveDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyUpdateDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyStatusDTO;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.j4.frequency.FrequencyOptionVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

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
public class FrequencyServiceImpl extends ServiceImpl<FrequencyMapper, CusFrequencyDO> implements IFrequencyService {
    @Resource
    private MsFrequencyMapper msFrequencyMapper;

    @Override
    public PageDTO<FrequencyVO> queryFrequencyPage(FrequencyPageQuery query) {
        // 构建分页对象
        Page<CusFrequencyDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        // 构建查询条件
        LambdaQueryWrapper<CusFrequencyDO> wrapper = new LambdaQueryWrapper<>();
        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(CusFrequencyDO::getName, query.getKeyword())
                    .or()
                    .like(CusFrequencyDO::getRateCode, query.getKeyword())
            );
        }
        // 按排序号升序排列
        wrapper.orderByAsc(CusFrequencyDO::getSortNo);
        // 执行分页查询
        Page<CusFrequencyDO> result = baseMapper.selectPage(page, wrapper);
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
        // 1. DTO转实体
        CusFrequencyDO entity = msFrequencyMapper.saveDtoToEntity(dto);
        // 2. 保存到数据库
        baseMapper.insert(entity);
        // 3. 实体转VO返回
        return msFrequencyMapper.entityToVo(entity);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public FrequencyVO modifyFrequency(FrequencyUpdateDTO dto) {
        // 1. 检查记录是否存在
        CusFrequencyDO existing = baseMapper.selectById(dto.getId());
        if (existing == null) {
            throw new RuntimeException("频次记录不存在");
        }
        // 2. DTO转实体
        CusFrequencyDO entity = msFrequencyMapper.updateDtoToEntity(dto);
        // 3. 保留系统字段
        entity.setCreatedAt(existing.getCreatedAt());
        entity.setCreatedBy(existing.getCreatedBy());
        entity.setIsDeleted(existing.getIsDeleted());
        entity.setDeletedAt(existing.getDeletedAt());
        entity.setDeletedBy(existing.getDeletedBy());
        entity.setVersion(existing.getVersion());
        // 4. 更新数据库
        baseMapper.updateById(entity);
        // 5. 查询最新数据并返回
        return msFrequencyMapper.entityToVo(baseMapper.selectById(dto.getId()));
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void removeFrequency(String id) {
        // 1. 检查记录是否存在
        CusFrequencyDO existing = baseMapper.selectById(id);
        if (existing == null) {
            throw new RuntimeException("频次记录不存在");
        }
        // 2. 逻辑删除（实体有@TableLogic注解，会自动转为update操作）
        baseMapper.deleteById(id);
    }

    @Override
    public List<FrequencyOptionVO> listFrequencyNames() {
        List<CusFrequencyDO> frequencies = lambdaQuery()
                .eq(CusFrequencyDO::getActiveFlag, (byte) 1)
                .orderByAsc(CusFrequencyDO::getSortNo)
                .list();

        return msFrequencyMapper.entitiesToOptionVos(frequencies);
    }

    @Override
    public boolean modifyFrequencyStatus(FrequencyStatusDTO dto) {
        return lambdaUpdate()
                .eq(CusFrequencyDO::getId, dto.getId())
                .set(CusFrequencyDO::getActiveFlag, dto.getActiveFlag())
                .update();
    }
}
