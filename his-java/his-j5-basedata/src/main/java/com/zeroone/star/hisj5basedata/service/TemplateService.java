package com.zeroone.star.hisj5basedata.service;

public interface TemplateService {

    /**
     * 生成模板
     *
     * @param templateName 模板名称
     * @param data         数据
     * @return 模板内容
     */
    String getTemplate(String templateName, Object data);
}
