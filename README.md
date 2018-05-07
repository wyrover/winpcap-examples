# winpcap-examples


- example-001 获取本地适配器信息
- example-002 获取已安装设备的高级信息
- example-003 打开适配器并捕获数据包
- example-004 不用回调方法获取数据包
- example-005 过滤及分析数据包
- example-006 处理脱机 dump 文件 (上)- 写入
- example-007 处理脱机 dump 文件 (上)- 读取
- example-008 处理脱机 dump 文件 (下)- 写入
- example-009 处理脱机 dump 文件 (下)- 读取
- example-010 发送数据包 (1)
- example-011 发送数据包 (2)
- example-012 收集并统计网络流量
- example-013 发送以太包
- example-014 发送 ARP 包
- example-015 发送 IP 包
- example-016 发送 TCP 包
- example-017 发送 ICMP 包
- example-018 使用 libnet 发包
- example-018 dump 包到文本文件

  VPN 劫持

- Layer2-Pivoting-Client   https://github.com/rsmudge/Layer2-Pivoting-Client  

  用 winpcap 在链路层抓包与 linux 的 tun 设备互联


- VPN_Pivoting https://github.com/imgits/VPN_Pivoting

  在 win 下使用 tap 设备建立的 vpn 隧道，同样依赖 winpcap 来发包



- simpletun

  一个在 linux 的下隧道程序，包含服务端口和客户端，在 windows 下可用 Layer2-Pivoting-Client 在链路层连 simpletun