package com.zeroone.star.j3.fee.service.impl;

import com.alibaba.cloud.commons.lang.StringUtils;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j3.fee.entity.FeeNature;
import com.zeroone.star.j3.fee.mapper.FeeNatureMapper;
import com.zeroone.star.j3.fee.service.IFeeNatureService;
import com.zeroone.star.project.dto.j3.fee.FeeNatureDTO;
import com.zeroone.star.project.enums.j3.FeeCategoryEnum;
import com.zeroone.star.project.query.j3.fee.FeeNatureQuery;
import com.zeroone.star.project.vo.j3.fee.FeeNatureVO;
import org.springframework.beans.BeanUtils;
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
    public boolean updateFeeNature(Long id, FeeNatureDTO dto) {
        FeeNature entity = baseMapper.selectById(id);
        if (entity == null) {
            return false;
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
        return updateById(entity);
    }

    @Override
    public boolean updateStatus(Long id, Integer isEnabled) {
        FeeNature entity = baseMapper.selectById(id);
        if (entity == null) {
            return false;
        }
        entity.setIsEnabled(isEnabled);
        return updateById(entity);
    }

    @Override
    public List<FeeNatureVO> getNameList() {
        // 注意是否启用,按照sortBy字段排序
        LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(FeeNature::getIsEnabled, 1)
                .orderByAsc(FeeNature::getSortOrder)
                .select(FeeNature::getCode, FeeNature::getName);
        List<FeeNature> feeNatures = baseMapper.selectList(wrapper);
        List<FeeNatureVO> vos = new ArrayList<>();
        for (FeeNature feeNature : feeNatures) {
            FeeNatureVO feeNatureVO = new FeeNatureVO();
            feeNatureVO.setName(feeNature.getName());
            feeNatureVO.setCode(feeNature.getCode());
            vos.add(feeNatureVO);
        }
        return vos;
    }

    @Override
    public void add(FeeNatureDTO dto) {
        // 1.校验编码
        // 1.1 编码不能为空
        String code = dto.getCode();
        if(StringUtils.isEmpty(code)){
            throw new RuntimeException("费用性质编码不能为空");
        }
        // 1.2  编码不能重复
        LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(FeeNature::getCode, dto.getCode());
        if (baseMapper.selectCount(wrapper) > 0) {
            throw new RuntimeException("费用性质编码已存在，请重新输入");
        }

        // 2. 判名称,分类不能为空
        String name = dto.getName();
        if(StringUtils.isEmpty(name)){
            throw new RuntimeException("费用性质名称不能为空");
        }

        // 3. 填充字段,保存
        FeeNature entity = new FeeNature();
        BeanUtils.copyProperties(dto,entity);
        entity.setIsEnabled(1);
        baseMapper.insert(entity);
    }

    @Override
    public void delete(Long id) {

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
