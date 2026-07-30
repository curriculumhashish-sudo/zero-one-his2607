package com.zeroone.star.configuration.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.emr.configmanage.dto.ExecPlaceDTO;
import com.zeroone.star.project.emr.configmanage.entity.ExecPlace;
import com.zeroone.star.project.emr.configmanage.query.ExecPlaceQuery;
import com.zeroone.star.project.emr.configmanage.vo.ExecPlaceVO;
import com.zeroone.star.configuration.mapper.ExecPlaceMapper;
import com.zeroone.star.configuration.service.IExecPlaceService;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class ExecPlaceServiceImpl extends ServiceImpl<ExecPlaceMapper, ExecPlace> implements IExecPlaceService {

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> add(ExecPlaceDTO dto) {
        if (!StringUtils.hasText(dto.getPlaceCode())) {
            return JsonVO.fail("执行地点编码不能为空");
        }
        if (!StringUtils.hasText(dto.getPlaceName())) {
            return JsonVO.fail("执行地点名称不能为空");
        }
        long count = count(new LambdaQueryWrapper<ExecPlace>()
                .eq(ExecPlace::getPlaceCode, dto.getPlaceCode())
                .eq(ExecPlace::getIsDeleted, 0));
        if (count > 0) {
            return JsonVO.fail("执行地点编码已存在");
        }
        ExecPlace entity = new ExecPlace();
        BeanUtil.copyProperties(dto, entity);
        entity.setCreatedAt(LocalDateTime.now());
        entity.setIsDeleted(0);
        entity.setVersion(0);
        save(entity);
        return JsonVO.success("新增成功");
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> update(ExecPlaceDTO dto) {
        if (dto.getId() == null) {
            return JsonVO.fail("主键不能为空");
        }
        if (!StringUtils.hasText(dto.getPlaceCode())) {
            return JsonVO.fail("执行地点编码不能为空");
        }
        if (!StringUtils.hasText(dto.getPlaceName())) {
            return JsonVO.fail("执行地点名称不能为空");
        }
        long count = count(new LambdaQueryWrapper<ExecPlace>()
                .eq(ExecPlace::getPlaceCode, dto.getPlaceCode())
                .eq(ExecPlace::getIsDeleted, 0)
                .ne(ExecPlace::getId, dto.getId()));
        if (count > 0) {
            return JsonVO.fail("执行地点编码已存在");
        }
        ExecPlace entity = getById(dto.getId());
        if (entity == null || entity.getIsDeleted() == 1) {
            return JsonVO.fail("执行地点不存在或已删除");
        }
        BeanUtil.copyProperties(dto, entity);
        entity.setUpdatedAt(LocalDateTime.now());
        updateById(entity);
        return JsonVO.success("修改成功");
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<String> delete(Long id) {
        if (id == null) {
            return JsonVO.fail("主键不能为空");
        }
        ExecPlace entity = getById(id);
        if (entity == null || entity.getIsDeleted() == 1) {
            return JsonVO.fail("执行地点不存在或已删除");
        }
        entity.setIsDeleted(1);
        entity.setUpdatedAt(LocalDateTime.now());
        updateById(entity);
        return JsonVO.success("删除成功");
    }

    @Override
    public JsonVO<PageDTO<ExecPlaceVO>> queryByPage(ExecPlaceQuery query) {
        LambdaQueryWrapper<ExecPlace> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(ExecPlace::getIsDeleted, 0);
        if (StringUtils.hasText(query.getPlaceCode())) {
            wrapper.like(ExecPlace::getPlaceCode, query.getPlaceCode());
        }
        if (StringUtils.hasText(query.getPlaceName())) {
            wrapper.like(ExecPlace::getPlaceName, query.getPlaceName());
        }
        if (query.getActiveFlag() != null) {
            wrapper.eq(ExecPlace::getActiveFlag, query.getActiveFlag());
        }
        wrapper.orderByAsc(ExecPlace::getSortOrder);
        Page<ExecPlace> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<ExecPlace> result = page(page, wrapper);
        PageDTO<ExecPlaceVO> pageDTO = convertToPageDTO(result);
        return JsonVO.success(pageDTO);
    }

    @Override
    public JsonVO<ExecPlaceVO> queryById(Long id) {
        if (id == null) {
            return JsonVO.fail("主键不能为空");
        }
        ExecPlace entity = getById(id);
        if (entity == null || entity.getIsDeleted() == 1) {
            return JsonVO.fail("执行地点不存在或已删除");
        }
        ExecPlaceVO vo = new ExecPlaceVO();
        BeanUtil.copyProperties(entity, vo);
        return JsonVO.success(vo);
    }

    private PageDTO<ExecPlaceVO> convertToPageDTO(Page<ExecPlace> page) {
        List<ExecPlaceVO> voList = page.getRecords().stream().map(entity -> {
            ExecPlaceVO vo = new ExecPlaceVO();
            BeanUtil.copyProperties(entity, vo);
            return vo;
        }).collect(Collectors.toList());
        PageDTO<ExecPlaceVO> pageDTO = new PageDTO<>();
        pageDTO.setTotal(page.getTotal());
        pageDTO.setPageIndex(page.getCurrent());
        pageDTO.setPageSize(page.getSize());
        pageDTO.setPages(page.getPages());
        pageDTO.setRows(voList);
        return pageDTO;
    }
}
