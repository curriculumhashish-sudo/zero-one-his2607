#!/bin/sh
# Nacos 配置初始化脚本
# 在 nacos-init 容器中执行，向 Nacos 发布 data-source.yaml 配置

NACOS_HOST="nacos"
NACOS_PORT="8848"
NACOS_USER="nacos"
NACOS_PASS="nacos"
CONFIG_DIR="/configs"

echo "============================================"
echo "  Nacos 配置初始化"
echo "============================================"

# 尝试登录获取 accessToken（如果 Nacos 开启了认证）
TOKEN=""
TOKEN_RESP=$(curl -sf -X POST "http://${NACOS_HOST}:${NACOS_PORT}/nacos/v1/auth/login" \
  -d "username=${NACOS_USER}&password=${NACOS_PASS}" 2>/dev/null) || true

if [ -n "$TOKEN_RESP" ]; then
    TOKEN=$(echo "$TOKEN_RESP" | sed -n 's/.*"accessToken":"\([^"]*\)".*/\1/p')
    if [ -n "$TOKEN" ]; then
        echo "已获取 Nacos accessToken"
    fi
fi

# 发布配置的函数
publish_config() {
    DATA_ID="$1"
    GROUP="$2"
    FILE_PATH="$3"
    CONTENT=$(cat "$FILE_PATH")

    echo "正在发布配置: dataId=${DATA_ID}, group=${GROUP}"

    if [ -n "$TOKEN" ]; then
        RESULT=$(curl -sf -X POST "http://${NACOS_HOST}:${NACOS_PORT}/nacos/v1/cs/configs?accessToken=${TOKEN}" \
            --data-urlencode "dataId=${DATA_ID}" \
            --data-urlencode "group=${GROUP}" \
            --data-urlencode "type=yaml" \
            --data-urlencode "content=${CONTENT}" 2>/dev/null)
    else
        RESULT=$(curl -sf -X POST "http://${NACOS_HOST}:${NACOS_PORT}/nacos/v1/cs/configs" \
            --data-urlencode "dataId=${DATA_ID}" \
            --data-urlencode "group=${GROUP}" \
            --data-urlencode "type=yaml" \
            --data-urlencode "content=${CONTENT}" 2>/dev/null)
    fi

    if [ "$RESULT" = "true" ]; then
        echo "  [成功] ${DATA_ID} 发布完成"
    else
        echo "  [失败] ${DATA_ID} 发布失败，响应: ${RESULT}"
        exit 1
    fi
}

# 发布 data-source.yaml（public namespace，不传 tenant 参数即为 public）
if [ -f "${CONFIG_DIR}/data-source.yaml" ]; then
    publish_config "data-source.yaml" "DEFAULT_GROUP" "${CONFIG_DIR}/data-source.yaml"
else
    echo "[警告] ${CONFIG_DIR}/data-source.yaml 不存在，跳过"
fi

echo "============================================"
echo "  Nacos 配置初始化完成"
echo "============================================"
