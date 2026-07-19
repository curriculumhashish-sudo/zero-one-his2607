package common;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "统一响应体")
public class Result<T> {

    @Schema(description = "状态码：200成功，400参数错误，500系统错误", example = "200", defaultValue = "200")
    private int code;

    @Schema(description = "提示信息", example = "success", defaultValue = "success")
    private String message;

    @Schema(description = "响应数据", defaultValue = "null")
    private T data;

    private Result(int code, String message, T data) {
        this.code = code; this.message = message; this.data = data;
    }

    public static <T> Result<T> ok(T data) { return new Result<>(200, "success", data); }
    public static <T> Result<T> fail(int code, String message) { return new Result<>(code, message, null); }

    public int getCode() { return code; }
    public void setCode(int code) { this.code = code; }
    public String getMessage() { return message; }
    public void setMessage(String message) { this.message = message; }
    public T getData() { return data; }
    public void setData(T data) { this.data = data; }
}
