package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.IFrequencyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencySaveDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyUpdateDTO;
import com.zeroone.star.project.j4.basedata.FrequencyApis;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：频次管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/frequency")
@Tag(name = "basedata-frequency", description = "频次管理接口")
public class FrequencyController implements FrequencyApis {

    @Resource
    private IFrequencyService frequencyService;

    @GetMapping("/query-frequency-list")
    @Override
    public JsonVO<PageDTO<FrequencyVO>> queryFrequencyList(FrequencyPageQuery query) {
        PageDTO<FrequencyVO> pageResult = frequencyService.queryFrequencyPage(query);
        return JsonVO.success(pageResult);
    }

    @GetMapping("/query-frequency-detail")
    @Override
    public JsonVO<FrequencyVO> queryFrequencyDetail(@RequestParam String id) {
        FrequencyVO detail = frequencyService.queryFrequencyDetail(id);
        if (detail == null) {
            return JsonVO.fail("频次记录不存在");
        }
        return JsonVO.success(detail);
    }

    @Operation(summary = "新增频次")
    @PostMapping("add-frequency")
    @Override
    public JsonVO<FrequencyVO> addFrequency(@Validated @RequestBody FrequencySaveDTO dto) {
        FrequencyVO vo = frequencyService.addFrequency(dto);
        return JsonVO.success(vo);
    }

    @Operation(summary = "修改频次")
    @PutMapping("modify-frequency")
    @Override
    public JsonVO<FrequencyVO> modifyFrequency(@Validated @RequestBody FrequencyUpdateDTO dto) {
        FrequencyVO vo = frequencyService.modifyFrequency(dto);
        return JsonVO.success(vo);
    }

    @Operation(summary = "删除频次")
    @DeleteMapping("remove-frequency/{id}")
    @Override
    public JsonVO<Void> removeFrequency(@PathVariable String id) {
        frequencyService.removeFrequency(id);
        return JsonVO.success(null);
    }
}
