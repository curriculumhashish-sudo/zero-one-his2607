package com.zeroone.star.ee.controller;

import com.zeroone.star.ee.entity.Document;
import com.zeroone.star.ee.mapper.DocumentMapper;
import io.swagger.v3.oas.annotations.Operation;
import jakarta.annotation.Resource;
import org.dromara.easyes.core.conditions.select.LambdaEsQueryWrapper;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：测试使用EasyEs控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
public class EeController {
    @Resource
    DocumentMapper mapper;

    @Operation(summary = "插入文档")
    @PostMapping("/insert")
    public Integer insert(String title, String content) {
        Document document = new Document();
        document.setTitle(title);
        document.setContent(content);
        return mapper.insert(document);
    }

    @Operation(summary = "查询文档")
    @GetMapping("/query")
    public List<Document> query(String title) {
        LambdaEsQueryWrapper<Document> wrapper = new LambdaEsQueryWrapper<>();
        wrapper.like(Document::getTitle, title);
        return mapper.selectList(wrapper);
    }
}
