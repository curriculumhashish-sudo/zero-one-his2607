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
                    .like(FeeNature::getCode, query.getKeyword()));
        }
        // 按显示顺序升序
        wrapper.orderByAsc(FeeNature::getSortOrder);

        List<FeeNature> entities = baseMapper.selectList(wrapper);
        List<FeeNatureVO> vos = new ArrayList<>();
        for (FeeNature entity : entities) {
            vos.add(toVO(entity));
        }
        return vos;
    }

    @Override
    public void updateFeeNature(Long id, FeeNatureDTO dto) {
        FeeNature entity = baseMapper.selectById(id);
        if (entity == null) {
            throw new RuntimeException("费用性质不存在");
        }
        // 编码变更时检查唯一性
        if (dto.getCode() != null && !dto.getCode().equals(entity.getCode())) {
            LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
            wrapper.eq(FeeNature::getCode, dto.getCode());
            if (baseMapper.selectCount(wrapper) > 0) {
                throw new RuntimeException("费用性质编码已存在，请重新输入");
            }
        }
        entity.setName(dto.getName());
        entity.setCode(dto.getCode());
        entity.setSortOrder(dto.getSortOrder());
        entity.setCategory(dto.getCategory());
        if (!updateById(entity)) {
            throw new RuntimeException("修改失败");
        }
    }

    @Override
    public void updateStatus(Long id, Integer isEnabled) {
        FeeNature entity = baseMapper.selectById(id);
        if (entity == null) {
            throw new RuntimeException("费用性质不存在");
        }
        entity.setIsEnabled(isEnabled);
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
        vo.setCode(entity.getCode());
        vo.setIsEnabled(entity.getIsEnabled());
        vo.setSortOrder(entity.getSortOrder());
        vo.setCategory(entity.getCategory());
        return vo;
    }
}
