// 屏幕回显输出方法
function writeToScreen(message, type = "info") {
    let pre = document.createElement("p");
    pre.style.overflowWrap = "break-word";

    // 添加样式类
    if (type === "info") {
        pre.classList.add("status-info");
    } else if (type === "error") {
        pre.classList.add("status-error");
    } else if (type === "received") {
        pre.classList.add("message-received");
    } else if (type === "sent") {
        pre.classList.add("message-sent");
    }

    pre.innerHTML = message;
    let output = document.getElementById("output");
    output.appendChild(pre);

    // 自动滚动到底部
    setTimeout(function () {
        output.scrollTop = output.scrollHeight;
    }, 10);

    // 更新在线状态
    updateOnlineStatus();
}

// 更新在线状态显示
function updateOnlineStatus() {
    const statusDot = document.querySelector(".online-dot");
    const statusText = document.querySelector(".online-status span");

    if (echo_websocket && echo_websocket.readyState === WebSocket.OPEN) {
        statusDot.classList.add("active");
        statusText.textContent = "在线";
    } else {
        statusDot.classList.remove("active");
        statusText.textContent = "离线";
    }
}

// 初始化websocket
let echo_websocket = null;

function init() {
    if (echo_websocket) return;

    // 获取输入的凭证
    let token = document.getElementById("token").value;
    if (!token) {
        alert("请输入登录凭证");
        return;
    }

    let wsUri = "ws://localhost:8090/chat"
    writeToScreen("正在连接到：" + wsUri, "info");

    try {
        // 1.创建WebSocket客户端对象
        echo_websocket = new WebSocket(wsUri, token);

        // 2.开门握手完成回调
        echo_websocket.onopen = function (evt) {
            console.log(evt);
            writeToScreen(`连接成功！可以开始聊天`, "info");
            updateOnlineStatus();
        };

        // 3.监听服务端的消息
        echo_websocket.onmessage = function (evt) {
            writeToScreen("接收消息: " + evt.data, "received");
        };

        // 4.如果连接中断
        echo_websocket.onerror = function (evt) {
            writeToScreen("连接错误: " + (evt.message || "未知错误"), "error");
            closeConnect();
        };

        // 5.注册close事件
        echo_websocket.onclose = function (evt) {
            writeToScreen("连接已关闭", "info");
            if (evt.reason) {
                writeToScreen(`关闭原因: ${evt.reason}`, "error");
            }
            echo_websocket = null;
            updateOnlineStatus();
        };
    } catch (e) {
        writeToScreen("连接失败: " + e.message, "error");
        echo_websocket = null;
        updateOnlineStatus();
    }
}

// 6.向服务发送消息
function send() {
    if (!echo_websocket) {
        alert("请先连接服务器");
        return;
    }

    let message = document.getElementById("message").value;
    let receiver = document.getElementById("receiverId").value;

    if (!receiver) {
        alert("请输入消息接收人的编号");
        return;
    }

    if (!message) {
        alert("请输入要发送的消息");
        return;
    }

    try {
        echo_websocket.send(receiver + "::" + message);
        writeToScreen("发送消息: " + message, "sent");
        // 清空消息框
        document.getElementById("message").value = "";
    } catch (e) {
        writeToScreen("发送失败: " + e.message, "error");
    }
}

// 7.关闭连接
function closeConnect() {
    if (!echo_websocket) return;
    try {
        echo_websocket.close();
        echo_websocket = null;
        updateOnlineStatus();
    } catch (e) {
        writeToScreen("关闭连接时出错: " + e.message, "error");
    }
}