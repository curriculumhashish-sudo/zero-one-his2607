package com.zeroone.star.j5.configmanage.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j5.configmanage.dto.ExecPlaceDTO;
import com.zeroone.star.j5.configmanage.entity.ExecPlace;
import com.zeroone.star.j5.configmanage.mapper.ExecPlaceMapper;
import com.zeroone.star.j5.configmanage.service.IExecPlaceService;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;

/**
 * <p>
 * 执行地点 服务实现类
 * </p>
 * @author 阿伟学长
 */
@Service
public class ExecPlaceServiceImpl extends ServiceImpl<ExecPlaceMapper, ExecPlace> implements IExecPlaceService {

    @Override
    public JsonVO<String> add(ExecPlaceDTO dto) {
        if (dto.getPlaceCode() == null || dto.getPlaceCode().isBlank()) {
            return JsonVO.fail("执行地点编码不能为空");
        }
        if (dto.getPlaceName() == null || dto.getPlaceName().isBlank()) {
            return JsonVO.fail("执行地点名称不能为空");
        }
        // 校验 placeCode 唯一性
        long count = count(new LambdaQueryWrapper<ExecPlace>()
                .eq(ExecPlace::getPlaceCode, dto.getPlaceCode()));
        if (count > 0) {
            return JsonVO.fail("执行地点编码已存在");
        }
        ExecPlace entity = new ExecPlace();
        BeanUtils.copyProperties(dto, entity);
        entity.setCreatedAt(LocalDateTime.now());
        entity.setIsDeleted(0);
        entity.setVersion(0);
        save(entity);
        return JsonVO.success("新增成功");
    }

    @Override
    public JsonVO<String> update(ExecPlaceDTO dto) {
        if (dto.getId() == null) {
            return JsonVO.fail("主键不能为空");
        }
        if (dto.getPlaceCode() == null || dto.getPlaceCode().isBlank()) {
            return JsonVO.fail("执行地点编码不能为空");
        }
        if (dto.getPlaceName() == null || dto.getPlaceName().isBlank()) {
            return JsonVO.fail("执行地点名称不能为空");
        }
        // 校验 placeCode 唯一性（排除自身）
        long count = count(new LambdaQueryWrapper<ExecPlace>()
                .eq(ExecPlace::getPlaceCode, dto.getPlaceCode())
                .ne(ExecPlace::getId, dto.getId()));
        if (count > 0) {
            return JsonVO.fail("执行地点编码已存在");
        }
        ExecPlace entity = new ExecPlace();
        BeanUtils.copyProperties(dto, entity);
        entity.setUpdatedAt(LocalDateTime.now());
        updateById(entity);
        return JsonVO.success("修改成功");
    }

    @Override
    public JsonVO<String> delete(Long id) {
        if (id == null) {
            return JsonVO.fail("主键不能为空");
        }
        removeById(id);
        return JsonVO.success("删除成功");
    }
}
