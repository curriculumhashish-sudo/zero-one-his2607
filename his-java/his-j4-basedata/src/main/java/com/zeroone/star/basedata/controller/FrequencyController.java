package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.IFrequencyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyStatusDTO;
import com.zeroone.star.project.j4.basedata.FrequencyApis;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyOptionVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

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

    @Operation(summary = "获取频次名称", description = "获取所有已启用的频次名称")
    @GetMapping("/list-frequency-names")
    @Override
    public JsonVO<List<FrequencyOptionVO>> listFrequencyNames() {
        return JsonVO.success(frequencyService.listFrequencyNames());
    }

    @Operation(summary = "启/停用频次", description = "修改指定频次的启用状态")
    @PutMapping("/modify-frequency-status")
    @Override
    public JsonVO<Boolean> modifyFrequencyStatus(
            @Valid @RequestBody FrequencyStatusDTO dto) {

        boolean result = frequencyService.modifyFrequencyStatus(dto);
        if (!result) {
            return JsonVO.fail("频次记录不存在或状态修改失败");
        }
        return JsonVO.success(true);
    }
}
