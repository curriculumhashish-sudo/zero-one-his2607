package com.zeroone.star.dispense.docs;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.test.context.SpringBootTest;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;

@SpringBootTest(
        webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT,
        properties = {
                "spring.cloud.nacos.config.enabled=false",
                "spring.cloud.nacos.discovery.enabled=false",
                "spring.datasource.url=jdbc:mysql://127.0.0.1:3306/test",
                "spring.datasource.username=api-docs-test",
                "spring.datasource.password=api-docs-test",
                "spring.datasource.hikari.initialization-fail-timeout=-1"
        }
)
class ApiDocsSmokeTest {

    @Value("${local.server.port}")
    private int port;

    @Test
    void inpatientExportDocsDoNotExposeInternalFormatterFields() throws Exception {
        HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create("http://127.0.0.1:" + port + "/j7-dispense/v3/api-docs"))
                .GET()
                .build();

        HttpResponse<String> response = HttpClient.newHttpClient()
                .send(request, HttpResponse.BodyHandlers.ofString());

        assertEquals(200, response.statusCode());
        assertFalse(response.body().contains("DATE_TIME_FORMATTER"));
    }
}
