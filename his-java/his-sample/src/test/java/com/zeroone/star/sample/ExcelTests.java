package com.zeroone.star.sample;

import com.zeroone.star.project.components.excel.ExcelComponent;
import com.zeroone.star.project.components.fastdfs.FastDfsClientComponent;
import com.zeroone.star.project.components.fastdfs.FastDfsFileInfo;
import com.zeroone.star.sample.test.entiy.User;
import jakarta.annotation.Resource;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.test.context.SpringBootTest;

import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.List;

@SpringBootTest
public class ExcelTests {

    @Resource
    ExcelComponent excel;

    final String path = "test.xlsx";

    @Test
    void testExport() {
        // 创建数据
        List<User> dataList = new ArrayList<>();
        for (int i = 1; i < 11; i++) {
            User user = new User();
            user.setId(i);
            user.setName("测试用户" + i);
            user.setPhone("1234567890" + i);
            dataList.add(user);
        }
        // 导出
        excel.export(path, "sheet1", User.class, dataList);
    }

    @Test
    void testParse() {
        List<User> users = excel.parseExcel(path, "sheet1", User.class);
        users.forEach(System.out::println);
    }

    @Resource
    FastDfsClientComponent dfs;
    @Value("${fastdfs.nginx-servers}")
    String serverUrl;

    @Test
    void testUpload() throws Exception {
        // 创建数据
        List<User> dataList = new ArrayList<>();
        for (int i = 1; i < 11; i++) {
            User user = new User();
            user.setId(i);
            user.setName("dfs用户" + i);
            user.setPhone("1234567890" + i);
            dataList.add(user);
        }
        // 定义输出流
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        excel.export("sheet1", out, User.class, dataList);
        // 上传到FastDFS
        FastDfsFileInfo info = dfs.uploadFile(out.toByteArray(), "xlsx");
        out.close();
        System.out.println(dfs.fetchUrl(info, "http://" + serverUrl, true));
    }
}
