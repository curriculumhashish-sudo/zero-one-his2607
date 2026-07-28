package com.zeroone.star.configuration.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.medicalconsumable.BindingDTO;
import com.zeroone.star.project.j5.medicalconsumable.BindingApis;
import com.zeroone.star.project.query.PageQuery;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.web.bind.annotation.*;

@RestController
public class BindingController implements BindingApis {

    @Override
    public JsonVO<PageDTO<BindingDTO>> queryPage(PageQuery pageQuery) {
        return null;  // 后续实现
    }

    @Override
    public JsonVO<String> addBinding(@RequestBody BindingDTO data) {
        return null;  // 后续实现
    }

    @Override
    public JsonVO<String> updateBinding(@RequestBody BindingDTO data) {
        return null;  // 后续实现
    }

    @Override
    public JsonVO<String> delBinding(@PathVariable String id) {
        return null;  // 后续实现
    }

    @Override
    public JsonVO<String> toggleStatus(@RequestBody BindingDTO data) {
        return null;  // 后续实现
    }
}
