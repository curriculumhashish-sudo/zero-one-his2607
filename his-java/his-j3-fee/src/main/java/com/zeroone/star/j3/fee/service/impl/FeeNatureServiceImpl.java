package com.zeroone.star.j3.fee.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j3.fee.entity.FeeNature;
import com.zeroone.star.j3.fee.mapper.FeeNatureMapper;
import com.zeroone.star.j3.fee.service.IFeeNatureService;
import com.zeroone.star.project.dto.j3.fee.FeeNatureDTO;
import com.zeroone.star.project.query.j3.fee.FeeNatureQuery;
import com.zeroone.star.project.vo.j3.fee.FeeNatureVO;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 费用性质 服务实现类
 * </p>
 * @author 白文儿
 * @version 1.0.0
 */
@Service
public class FeeNatureServiceImpl extends ServiceImpl<FeeNatureMapper, FeeNature> implements IFeeNatureService {

    @Override
    public List<FeeNatureVO> listFeeNatures(FeeNatureQuery query) {
        LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
        // 关键词模糊搜索（名称或编码）
        if (query.getKeyword() != null && !query.getKeyword().isEmpty()) {
            wrapper.and(w -> w
                    .like(FeeNature::getName, query.getKeyword())
                    .or()
                    .like(FeeNature::getBusNo, query.getKeyword()));
        }
        // 按显示顺序升序
        wrapper.orderByAsc(FeeNature::getDisplayOrder);

        List<FeeNature> entities = baseMapper.selectList(wrapper);
        List<FeeNatureVO> vos = new ArrayList<>();
        for (FeeNature entity : entities) {
            vos.add(toVO(entity));
        }
        return vos;
    }

    @Override
    public void updateFeeNature(String id, FeeNatureDTO dto) {
        FeeNature entity = baseMapper.selectById(id);
        if (entity == null) {
            throw new RuntimeException("费用性质不存在");
        }
        // 编码变更时检查唯一性
        if (dto.getBusNo() != null && !dto.getBusNo().equals(entity.getBusNo())) {
            LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
            wrapper.eq(FeeNature::getBusNo, dto.getBusNo());
            if (baseMapper.selectCount(wrapper) > 0) {
                throw new RuntimeException("费用性质编码已存在，请重新输入");
            }
        }
        entity.setName(dto.getName());
        entity.setBusNo(dto.getBusNo());
        entity.setDisplayOrder(dto.getDisplayOrder());
        entity.setCategoryEnum(dto.getCategoryEnum());
        entity.setActiveFlag(dto.getActiveFlag());
        if (!updateById(entity)) {
            throw new RuntimeException("修改失败");
        }
    }

    @Override
    public void updateStatus(String id, Integer isEnabled) {
        FeeNature entity = baseMapper.selectById(id);
        if (entity == null) {
            throw new RuntimeException("费用性质不存在");
        }
        entity.setActiveFlag(isEnabled);
        if (!updateById(entity)) {
            throw new RuntimeException("状态更新失败");
        }
    }

    /**
     * Entity 转 VO
     */
    private FeeNatureVO toVO(FeeNature entity) {
        FeeNatureVO vo = new FeeNatureVO();
        vo.setId(entity.getId());
        vo.setName(entity.getName());
        vo.setBusNo(entity.getBusNo());
        vo.setActiveFlag(entity.getActiveFlag());
        vo.setDisplayOrder(entity.getDisplayOrder());
        vo.setCategoryEnum(entity.getCategoryEnum());
        vo.setStatusEnum(entity.getStatusEnum());
        return vo;
    }
}
