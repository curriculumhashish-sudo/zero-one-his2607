package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.controller.IBodyPartController;
import com.zeroone.star.project.dto.BodyPartAddDTO;
import com.zeroone.star.project.query.BodyPartPageQuery;
import com.zeroone.star.project.vo.BodyPartListVO;
import com.zeroone.star.project.vo.BodyPartTreeVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

import javax.validation.Valid;
import java.util.ArrayList;
import java.util.List;

/**
 * 部位管理 Controller
 */
@RestController
@RequestMapping("/basedata/body-part")
@Tag(name = "部位管理")
@RequiredArgsConstructor
public class BodyPartController implements IBodyPartController {

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