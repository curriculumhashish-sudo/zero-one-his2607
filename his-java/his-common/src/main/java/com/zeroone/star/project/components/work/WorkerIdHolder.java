package com.zeroone.star.project.components.work;

import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.lang3.RandomUtils;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.CommandLineRunner;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Component;

import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

/**
 * 机器workerId初始化工具
 * @author 是丫丫
 * @date 2026/7/24
 */
@Component
@Slf4j
public class WorkerIdHolder implements CommandLineRunner {

    /**
     * 全局workerId
     */
    public static long workerId;

    /**
     * 默认16台机器
     */
    @Value("${worker.id.workNum:16}")
    private long workerMaxNum;

    /**
     * redis键
     */
    @Value("${worker.id.key:workerId}")
    private String workerIdKey;

    @Resource
    private RedisTemplate<String, Long>  redisTemplate;

    /**
     * WorkerIdHolder实例化完成后运行
     * @param args incoming main method arguments
     * @throws Exception 异常
     */
    @Override
    public void run(String... args) throws Exception {

        // 不得大于32
        workerMaxNum = workerMaxNum > 32 ? 32 : workerMaxNum;

        Long incrNum = null;
        int maxNum = 10;
        while (incrNum == null && maxNum-- > 0) {
            try {
                incrNum = redisTemplate.opsForValue().increment(workerIdKey, 1L);
            } catch (Exception e) {
                log.error("Redis获取workerId失败，剩余重试次数:{}", maxNum, e);
            }
        }
        // 雪花算法workId
        if (incrNum == null) {
            workerId = getWorkerIdByMacAddress(workerMaxNum);
            log.warn("workId 初始化失败， 使用机器网卡MAC地址生成workId：{}", workerId);
            return;
        }
        workerId = incrNum % workerMaxNum;
        log.info("workId 初始化完成: 本机器workId为： {}", workerId);
    }

    /**
     * 兜底：读取本机网卡MAC地址计算workerId <br>
     * &#064;SuppressWarnings  消除nextLong弃用警告
     * @param maxNum worker最大数量
     * @return 0 ~ maxNum-1 的workerId
     */
    @SuppressWarnings("deprecation")
    private long getWorkerIdByMacAddress(long maxNum) {
        // 兜底随机数workerId
        long workerId = RandomUtils.nextLong(0, maxNum);
        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface ni = interfaces.nextElement();
                // 必过滤：未启用、本地回环、虚拟网卡
                if (!ni.isUp() || ni.isLoopback() || ni.isVirtual()) {
                    continue;
                }

                byte[] mac = ni.getHardwareAddress();
                // MAC长度不足2字节跳过
                if (mac == null || mac.length < 2) {
                    continue;
                }

                log.info("网卡信息: {}", ni);
                long calcId = ((0x000000FF & (long) mac[mac.length - 2])
                        | (0x0000FF00 & (((long) mac[mac.length - 1]) << 8))) >> 6;

                // 取模限制区间，绝对值保证非负
                return Math.abs(calcId % maxNum);
            }
        } catch (SocketException e) {
            log.error("读取网卡信息异常，兜底随机数workerId: {}", workerId);
            return workerId;
        }
        log.info("未读取到有效网卡，兜底随机数workerId: {}", workerId);
        return workerId;
    }
}
