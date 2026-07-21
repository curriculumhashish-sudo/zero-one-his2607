package com.zeroone.star.basedata.controller;


import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartAddDTO;
import com.zeroone.star.project.j2.basedata.qingzhou.IBodyPartApis;

import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;

import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartTreeVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;


import java.util.ArrayList;
import java.util.List;

/**
 * 部位管理 Controller
 */
@RestController
@RequestMapping("/basedata/body-part")
@Tag(name = "部位管理")
@RequiredArgsConstructor
public class BodyPartController implements IBodyPartApis {
    @Override
    public List<BodyPartTreeVO> getTree() {
        // TODO 后续实现从数据库获取部位树
        return new ArrayList<>();
    }

    @Override
    public List<BodyPartListVO> getList(@Valid BodyPartPageQuery query) {
        // TODO 后续实现分页条件查询
        return new ArrayList<>();
    }

    @Override
    public Long add(@Valid @RequestBody BodyPartAddDTO dto) {
        // TODO 后续实现新增部位
        return null;
    }
}