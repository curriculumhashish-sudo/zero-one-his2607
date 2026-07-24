package com.zeroone.star.cpp.service;


import cn.hutool.db.Page;
import com.zeroone.star.cpp.config.FeignConfiguration;
import com.zeroone.star.cpp.factory.SampleServiceFallBack;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.sample.SampleAddDTO;
import com.zeroone.star.project.dto.sample.SampleDTO;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;
@FeignClient(name = "${cpp.sample.name}", path = "${cpp.sample.url}", fallbackFactory = SampleServiceFallBack.class,configuration = FeignConfiguration.class)
public interface ISampleService {

    @GetMapping("/sample")
    JsonVO<PageDTO<SampleDTO>> get(@RequestParam Map<String,Object> condition);

    @PostMapping("/sample")
    JsonVO<String> post(@RequestBody SampleAddDTO dto);

    @PutMapping("/sample")
    JsonVO<String> put(@RequestBody SampleDTO dto);

    @DeleteMapping("/sample")
    JsonVO<List<String>> delete(@RequestBody List<String> ids);

}
