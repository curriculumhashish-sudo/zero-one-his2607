package com.zeroone.star.j3.register.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j3.register.entity.HealthcareService;
import com.zeroone.star.j3.register.mapper.HealthcareServiceMapper;
import com.zeroone.star.j3.register.service.IRegisterTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.register.RegisterQuery;
import com.zeroone.star.project.vo.j3.register.RegisterNameVO;
import com.zeroone.star.project.vo.j3.register.RegisterVO;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * <p>
 * 挂号类型 服务实现类（基于 adm_healthcare_service + adm_charge_item_definition）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Service
public class RegisterTypeServiceImpl extends ServiceImpl<HealthcareServiceMapper, HealthcareService> implements IRegisterTypeService {

    /**
     * 挂号类型在医疗服务表中的类别标记
     */
    private static final String CATEGORY_REGISTRATION = "REGISTRATION";

    @Override
    public PageDTO<RegisterVO> queryRegisterList(RegisterQuery query) {
        Page<RegisterVO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        baseMapper.selectRegisterPage(page, StrUtil.trimToNull(query.getKeyword()));
        return PageDTO.create(page);
    }

    @Override
    public List<RegisterNameVO> queryRegisterNames() {
        // 只查启用的挂号类型，仅取 id + name 两列（@TableLogic 自动过滤软删）
        return list(new LambdaQueryWrapper<HealthcareService>()
                .select(HealthcareService::getId, HealthcareService::getName)
                .eq(HealthcareService::getCategoryCode, CATEGORY_REGISTRATION)
                .eq(HealthcareService::getActiveFlag, 1)
                .orderByAsc(HealthcareService::getCreatedAt))
                .stream()
                .map(hs -> new RegisterNameVO(Long.valueOf(hs.getId()), hs.getName()))
                .toList();
    }
}
