package com.zeroone.star.cpp.service.impl;

import com.zeroone.star.cpp.service.ISampleService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.sample.SampleAddDTO;
import com.zeroone.star.project.dto.sample.SampleDTO;
import com.zeroone.star.project.vo.JsonVO;
import jakarta.annotation.Resource;
import lombok.AllArgsConstructor;

import java.util.List;
import java.util.Map;

@AllArgsConstructor
public class SampleServiceImpl implements ISampleService {

    private Throwable throwable;
    private<T> JsonVO<T> createFailVO(){
        JsonVO<T> vo = JsonVO.fail(null);
        if(throwable.getMessage() != null){
            vo.setMessage(throwable.getMessage());
        }else{
            vo.setMessage(throwable.getClass().toGenericString());
        }
        return vo;
    }

    @Override
    public JsonVO<PageDTO<SampleDTO>> get(Map<String, Object> condition) {
        return createFailVO();
    }

    @Override
    public JsonVO<String> post(SampleAddDTO dto) {
        return createFailVO();
    }

    @Override
    public JsonVO<String> put(SampleDTO dto) {
        return createFailVO();
    }

    @Override
    public JsonVO<List<String>> delete(List<String> ids) {
        return createFailVO();
    }
}
