package com.zeroone.star.j3.register.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j3.register.entity.RegisterType;
import com.zeroone.star.j3.register.mapper.RegisterTypeMapper;
import com.zeroone.star.j3.register.service.IRegisterTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j3.register.dto.RegisterDTO;
import com.zeroone.star.project.j3.register.query.RegisterQuery;
import com.zeroone.star.project.j3.register.vo.RegisterVO;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 描述：挂号类型服务实现类
 * </p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Service
public class RegisterTypeServiceImpl extends ServiceImpl<RegisterTypeMapper, RegisterType> implements IRegisterTypeService {

    @Override
    public PageDTO<RegisterVO> queryRegisterList(RegisterQuery query) {
        // 1 构建分页对象
        Page<RegisterType> page = new Page<>(query.getPageIndex(), query.getPageSize());
        // 2 构建查询条件
        LambdaQueryWrapper<RegisterType> wrapper = new LambdaQueryWrapper<>();
        String keyword = query.getKeyword();
        if (keyword != null && !keyword.trim().isEmpty()) {
            String kw = keyword.trim();
            wrapper.and(w -> w.like(RegisterType::getName, kw)
                    .or().like(RegisterType::getMdtrtCertNo, kw)
                    .or().like(RegisterType::getBusinessCode, kw));
            // 如果关键词为纯数字，同时按 id 精确匹配
            try {
                long id = Long.parseLong(kw);
                wrapper.or().eq(RegisterType::getId, id);
            } catch (NumberFormatException ignored) {
            }
        }
        // 按 id 倒序
        wrapper.orderByDesc(RegisterType::getId);
        // 3 查询
        page = baseMapper.selectPage(page, wrapper);
        // 4 转换并返回（RegisterType 与 RegisterVO 同名字段自动复制）
        return PageDTO.create(page, RegisterVO::new);
    }

    @Override
    public boolean addRegister(RegisterDTO dto) {
        RegisterType entity = new RegisterType();
        BeanUtil.copyProperties(dto, entity);
        return save(entity);
    }

    @Override
    public boolean modifyRegister(RegisterDTO dto) {
        if (dto.getId() == null) {
            return false;
        }
        RegisterType entity = new RegisterType();
        BeanUtil.copyProperties(dto, entity);
        return updateById(entity);
    }

    @Override
    public boolean removeRegister(Long id) {
        if (id == null) {
            return false;
        }
        return removeById(id);
    }
}
