package com.zeroone.star.mongo.controller;

import com.zeroone.star.mongo.service.MemberReadHistoryService;
import com.zeroone.star.project.dto.mongo.MemberReadHistoryDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.mongo.MemberReadHistoryVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：会员浏览历史记录管理
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@Tag(name = "会员浏览历史记录管理")
public class MemberReadHistoryController {
    @Resource
    private MemberReadHistoryService service;

    @PostMapping("/create")
    @Operation(summary = "创建会员浏览历史记录")
    public JsonVO<Integer> create(MemberReadHistoryDTO history) {
        return JsonVO.success(service.save(history));
    }

    @PostMapping("/delete")
    @Operation(summary = "删除会员浏览历史记录")
    public JsonVO<Integer> delete(@RequestBody List<String> ids) {
        return JsonVO.success(service.remove(ids));
    }

    @PostMapping("/list")
    @Operation(summary = "获取会员浏览历史记录列表")
    public JsonVO<List<MemberReadHistoryVO>> list(Long memberId) {
        return JsonVO.success(service.list(memberId));
    }
}
