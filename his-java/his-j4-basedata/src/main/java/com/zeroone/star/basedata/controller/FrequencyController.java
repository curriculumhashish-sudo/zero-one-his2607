package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.IFrequencyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j4.basedata.FrequencyApis;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

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
}
