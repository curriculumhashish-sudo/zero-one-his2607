package com.zeroone.star.j3.fee.service.impl;

import com.alibaba.cloud.commons.lang.StringUtils;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j3.fee.entity.AdmAccount;
import com.zeroone.star.j3.fee.entity.FeeNature;
import com.zeroone.star.j3.fee.mapper.AdmAccountMapper;
import com.zeroone.star.j3.fee.mapper.FeeNatureMapper;
import com.zeroone.star.j3.fee.service.IFeeNatureService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.fee.FeeNatureDTO;
import com.zeroone.star.project.enums.j3.fee.FeeCategoryEnum;
import com.zeroone.star.project.query.j3.fee.FeeNatureQuery;
import com.zeroone.star.project.vo.j3.fee.FeeNatureVO;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
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
@RequiredArgsConstructor
public class FeeNatureServiceImpl extends ServiceImpl<FeeNatureMapper, FeeNature> implements IFeeNatureService {

    private final AdmAccountMapper admAccountMapper;
    private final UserHolder userHolder;

    @Override
    public PageDTO<FeeNatureVO> listFeeNatures(FeeNatureQuery query) {
        LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
        // 过滤已删除的记录
        wrapper.eq(FeeNature::getIsDeleted, 0);
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

        // 先记录总数（分页前）
        long total = entities.size();

        // 内存分页截取
        int fromIndex = (int) ((query.getPageIndex() - 1) * query.getPageSize());
        int toIndex = (int) Math.min(fromIndex + query.getPageSize(), entities.size());

        // 防止越界
        if (fromIndex >= entities.size()) {
            PageDTO<FeeNatureVO> emptyPage = new PageDTO<>();
            emptyPage.setPageIndex(query.getPageIndex());
            emptyPage.setPageSize(query.getPageSize());
            emptyPage.setTotal(total);
            emptyPage.setPages((total + query.getPageSize() - 1) / query.getPageSize());
            emptyPage.setRows(new ArrayList<>());
            return emptyPage;
        }

        List<FeeNature> pagedEntities = entities.subList(fromIndex, toIndex);

        List<FeeNatureVO> vos = new ArrayList<>();
        for (FeeNature entity : pagedEntities) {
            vos.add(toVO(entity));
        }

        PageDTO<FeeNatureVO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(query.getPageIndex());
        pageDTO.setPageSize(query.getPageSize());
        pageDTO.setTotal(total);
        pageDTO.setPages((total + query.getPageSize() - 1) / query.getPageSize());
        pageDTO.setRows(vos);
        return pageDTO;
    }

    @Override
    public void updateFeeNature(String id, FeeNatureDTO dto) {
        // 1. 校验必填字段
        if (dto.getName() == null || dto.getName().isEmpty()) {
            throw new RuntimeException("名称不能为空");
        }
        if (dto.getBusNo() == null || dto.getBusNo().isEmpty()) {
            throw new RuntimeException("编码不能为空");
        }
        if (dto.getCategoryEnum() == null) {
            throw new RuntimeException("分类不能为空");
        }
        if (dto.getDisplayOrder() == null) {
            throw new RuntimeException("显示顺序不能为空");
        }

        // 2. 校验记录存在
        FeeNature entity = baseMapper.selectById(id);
        if (entity == null) {
            throw new RuntimeException("费用性质不存在");
        }

        // 3. 编码变更时检查唯一性
        if (!dto.getBusNo().equals(entity.getBusNo())) {
            LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
            wrapper.eq(FeeNature::getBusNo, dto.getBusNo());
            if (baseMapper.selectCount(wrapper) > 0) {
                throw new RuntimeException("费用性质编码已存在，请重新输入");
            }
        }

        // 4. 仅更新编辑页面涉及的4个字段
        entity.setName(dto.getName());
        entity.setBusNo(dto.getBusNo());
        entity.setDisplayOrder(dto.getDisplayOrder());
        entity.setCategoryEnum(FeeCategoryEnum.of(dto.getCategoryEnum()));

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
        // 同步更新状态枚举，保持数据一致性
        entity.setStatusEnum(isEnabled == 1 ? "1" : "0");
        if (!updateById(entity)) {
            throw new RuntimeException("状态更新失败");
        }
    }
    @Override
    public List<FeeNatureVO> getNameList() {
        // 注意是否启用,是否被删除,按照sortBy字段排序
        LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(FeeNature::getActiveFlag, 1)
                .eq(FeeNature::getIsDeleted, 0)
                .orderByAsc(FeeNature::getDisplayOrder)
                .select(FeeNature::getBusNo, FeeNature::getName);
        List<FeeNature> feeNatures = baseMapper.selectList(wrapper);
        List<FeeNatureVO> vos = new ArrayList<>();
        for (FeeNature feeNature : feeNatures) {
            FeeNatureVO feeNatureVO = new FeeNatureVO();
            feeNatureVO.setName(feeNature.getName());
            feeNatureVO.setBusNo(feeNature.getBusNo());
            vos.add(feeNatureVO);
        }
        return vos;
    }

    @Override
    public void add(FeeNatureDTO dto) {
        // 1.校验编码
        // 1.1 编码不能为空
        String code = dto.getBusNo();
        if(StringUtils.isEmpty(code)){
            throw new RuntimeException("费用性质编码不能为空");
        }
        // 1.2  编码不能重复
        LambdaQueryWrapper<FeeNature> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(FeeNature::getBusNo, dto.getBusNo());
        wrapper.eq(FeeNature::getIsDeleted, 0);
        if (baseMapper.selectCount(wrapper) > 0) {
            throw new RuntimeException("费用性质编码已存在，请重新输入");
        }

        // 2. 判名称,显示顺序,分类不能为空
        String name = dto.getName();
        if(StringUtils.isEmpty(name)){
            throw new RuntimeException("费用性质名称不能为空");
        }

        Integer displayOrder = dto.getDisplayOrder();
        if(displayOrder==null){
            throw new RuntimeException("费用性质显示顺序不能为空");
        }

        Integer categoryEnum = dto.getCategoryEnum();
        if (categoryEnum == null) {
            throw new RuntimeException("费用性质分类不能为空");
        }


        // 3. 填充字段,保存
        FeeNature entity = new FeeNature();
        BeanUtils.copyProperties(dto, entity);
        entity.setCategoryEnum(FeeCategoryEnum.of(categoryEnum));
        entity.setActiveFlag(1);
        entity.setStatusEnum("1");
        entity.setIsDeleted(0);
        baseMapper.insert(entity);
    }

    @Override
    public void delete(String id) {
        // 注意是否已经被删除,是否关联了其他表,是否还是启用状态
        // 1. 数据是否存在
        FeeNature entity = baseMapper.selectById(id);
        if (entity == null) {
            throw new RuntimeException("费用性质不存在");
        }

        // 2. 删除判断是否已经被删除了
        if(entity.getIsDeleted() == 1){
            throw new RuntimeException("请不要重复删除");
        }

        // 3. 关联表判断
        // 3.1 挂号账户判断
        LambdaQueryWrapper<AdmAccount> wrapper = new LambdaQueryWrapper<AdmAccount>()
                .eq(AdmAccount::getBusNo, entity.getBusNo())
                .eq(AdmAccount::getIsDeleted, 0)
                .eq(AdmAccount::getStatusEnum, "ACTIVE");
        if(admAccountMapper.selectCount(wrapper) > 0){
            throw new RuntimeException("费用性质在诊断时被使用,无法删除");
        }

        // 4.1 更新删除字段
        entity.setIsDeleted(1);
        entity.setDeletedAt(LocalDateTime.now());
        try {
            entity.setDeletedBy(userHolder.getCurrentUser().getUsername());
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        baseMapper.updateById(entity);
    }

    /**
     * Entity 转 VO
     */
    private FeeNatureVO toVO(FeeNature entity) {
        FeeNatureVO vo = new FeeNatureVO();
        vo.setId(entity.getId());
        vo.setName(entity.getName());
        vo.setBusNo(entity.getBusNo());
        vo.setDisplayOrder(entity.getDisplayOrder());
        vo.setActiveFlag(entity.getActiveFlag());
        return vo;
    }
}
