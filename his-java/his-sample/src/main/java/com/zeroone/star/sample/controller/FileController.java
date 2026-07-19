package com.zeroone.star.sample.controller;

import cn.hutool.core.date.DateTime;
import com.zeroone.star.project.components.fastdfs.FastDfsClientComponent;
import com.zeroone.star.project.components.fastdfs.FastDfsFileInfo;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.apache.ibatis.annotations.Options;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;

/**
 *
 * @author tangcz
 *
 */
@RestController
@RequestMapping("file")
@Tag(name = "文件上传下载")
@RequiredArgsConstructor
public class FileController {

    @Value("${fastdfs.nginx-servers}")
    private String serverUrl;

    private final FastDfsClientComponent dfs;


    @PostMapping(value = "upload", consumes = MediaType.MULTIPART_FORM_DATA_VALUE)
    @Operation(summary = "文件上传")
    @Parameter(name = "file", description = "文件", required = true, schema = @Schema(type = "string", format = "binary"))
    public JsonVO<String> uploadFile(MultipartFile file) throws Exception {
        // 获取上传文件的后缀名
        String extName = file.getOriginalFilename().substring(file.getOriginalFilename().lastIndexOf(".") + 1);
        // 上传文件
        FastDfsFileInfo info = dfs.uploadFile(file.getBytes(), extName);
        if (info != null)
            return JsonVO.success(dfs.fetchUrl(info, "http://" + serverUrl, true));
        return JsonVO.fail("文件上传失败");
    }

    @Operation(summary = "下载文件")
    @GetMapping(value = "download", produces = "image/jpeg")
    public ResponseEntity<byte[]> downloadFile(FastDfsFileInfo info) throws Exception {
        // 下载文件
        byte[] bytes = dfs.downloadFile(info);
        // 响应给前端
        HttpHeaders headers = new HttpHeaders();
        String filename = "img-" + DateTime.now().toString("yyyyMMddHHmmssS") + ".jpg";
        headers.setContentDispositionFormData("attachment", filename);
        headers.setContentType(MediaType.IMAGE_JPEG);
        return new ResponseEntity<>(bytes, headers, HttpStatus.CREATED);
    }

    @Operation(summary = "删除文件")
    @DeleteMapping("delete")
    @ResponseBody
    public JsonVO<Integer> deleteFile(FastDfsFileInfo info) throws Exception {
        int result = dfs.deleteFile(info);
        if (result == 0) {
            return JsonVO.success(result);
        }
        return JsonVO.fail(result);
    }
}





















