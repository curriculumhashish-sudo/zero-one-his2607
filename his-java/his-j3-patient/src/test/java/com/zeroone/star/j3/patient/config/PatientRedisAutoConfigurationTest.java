package com.zeroone.star.j3.patient.config;

import org.junit.jupiter.api.Test;
import org.springframework.boot.autoconfigure.AutoConfigurations;
import org.springframework.boot.autoconfigure.data.redis.RedisAutoConfiguration;
import org.springframework.boot.test.context.runner.ApplicationContextRunner;

import static org.assertj.core.api.Assertions.assertThat;

class PatientRedisAutoConfigurationTest {

    private final ApplicationContextRunner contextRunner = new ApplicationContextRunner()
            .withConfiguration(AutoConfigurations.of(RedisAutoConfiguration.class));

    @Test
    void shouldProvideRedisTemplateBean() {
        contextRunner.run(context -> assertThat(context).hasBean("redisTemplate"));
    }
}
