package com.zeroone.star.sample.controller;

import cn.hutool.core.date.DateTime;
import com.zeroone.star.project.components.excel.ExcelComponent;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sample.test.entiy.User;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.PostConstruct;
import jakarta.annotation.Resource;
import lombok.SneakyThrows;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.multipart.MultipartFile;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

@Controller
@RequestMapping("excel")
@Tag(name = "excel")
public class ExcelController {

    @Resource
    ExcelComponent excel;

    private List<User> users;

    @PostConstruct
    public void initData(){
        users = new ArrayList<>();
        for (int i = 1; i < 11; i++) {
            User user = new User();
            user.setId(i);
            user.setName("测试数据导出" + i);
            user.setPhone("1234567890" + i);
            users.add(user);
        }
    }

    @SneakyThrows
    @GetMapping(value = "export",produces = "application/octet-stream")
    @Operation(summary = "导出excel")
    public ResponseEntity<byte[]> export() throws IOException {
        // 定义输出流
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        excel.export("sheet1", out, User.class, users);
        byte[] bytes = out.toByteArray();
        out.close();
        // 响应给前端
        HttpHeaders headers = new HttpHeaders();
        String filename = "report-" + DateTime.now().toString("yyyyMMddHHmmssS") + ".xlsx";
        headers.setContentDispositionFormData("attachment", filename);
        headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
        return new ResponseEntity<>(bytes, headers, HttpStatus.CREATED);
    }

    @SneakyThrows
    @Operation(summary = "导入Excel")
    @Parameter(name = "file", description = "excel文件", required = true, schema = @Schema(type = "string", format = "binary"))
    @PostMapping(value = "import", consumes = MediaType.MULTIPART_FORM_DATA_VALUE)
    public JsonVO<String> importExcel(MultipartFile file) {
        // 解析Excel
        InputStream inputStream = file.getInputStream();
        List<User> users = excel.parseExcel(inputStream, "sheet1", User.class);
        // 保存到数据库
        // 直接将数据加入到users来模拟保存
        this.users.addAll(users);
        this.users.forEach(System.out::println);
        return JsonVO.success("导入成功");
    }

}
