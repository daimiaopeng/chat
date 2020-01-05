## 黏包分包 2020年1月5日09:44:53 ：

```bash
I0105 09:41:18.455325 16923 Server.cpp:95] 127.0.0.1 发来一条消息: {"code":2,"token":"qwe"}{"code":4,"token":"qwe"}
E0105 09:41:18.456482 16923 MessageJson.h:39] 消息格式错误: [json.exception.parse_error.101] parse error at line 1, column 25: syntax error while parsing value - unexpected '{'; expected end of input
```

### 分析：
客户端发送缓冲区把两条消息一同发送，服务器没有对tcp进行分包，不是导致缓冲区积压，而是客户端把两条消息变成一条发送。
###  解决办法：
服务器对消息进行分包处理。

###  修复状态：

​	无

------
