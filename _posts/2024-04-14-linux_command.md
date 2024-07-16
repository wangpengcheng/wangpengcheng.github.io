

# linux 日常命令收集

## 参考链接

## 排查命令

### [`sar`日志查询命令](https://www.cnblogs.com/zsql/p/11628766.html)

```bash
sar -s 22:40:00 -e 22:43:00 -A -f sa07 | more
```

### [`tcpdump`抓包](https://www.cnblogs.com/wongbingming/p/13212306.html)

```bash
tcpdump -iany host 10.225.156.67 -w 255.pcap
```

### [`grep`命令]()

```bash
 cat ./pod.txt | grep  -E "flink|matrix"  > pod1.txt
```

## 磁盘命令

### [`nvme`设备管理命令](https://zhuanlan.zhihu.com/p/667230252)

此命令常用来，进行nvme设备管理

```bash
nvme smart-log /dev/nvme0n1;   nvme intel smart-log-add /dev/nvme0n1;
```

## kuberenetes


### kubectl 

```bash
kubectl get pods --all-namespaces -owide | grep sp4-cknode-1970
```

## 网络相关

### [`iptable`网络防火墙](https://lixiangyun.gitbook.io/iptables_doc_zh_cn)

```bash
nsenter -n  -t $pid # 将进程pid ip table 加载到宿主机中
```

## 存储相关

### 磁盘清理

磁盘清理流程：
- df -h 查看分区使用情况
- du -sh * 逐级寻找占用空间比较大的目录
- 找到对应目录之后  find . -name “*.log*”-mtime +5 |xargs rm -rf


## Pref

### 基础性能调优

```bash
perf stat -e L1-dcache-load-misses,L1-dcache-loads,L1-dcache-stores,L1-icache-load-misses,L1-icache-loads,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,LLC-prefetch-misses
```
## 内核相关

### 内核模块

- [内核模块管理](https://github.com/ytinirt/notes/blob/master/docs/os.md#%E5%86%85%E6%A0%B8%E6%A8%A1%E5%9D%97module)
- [内核模块加载](https://blog.exsvc.cn/article/centos-module-install-remove.html)

```bash
# 1. 检查模块已被静态编译到内核中：
grep -e ipvs -e nf_conntrack_ipv4 /lib/modules/$(uname -r)/modules.builtin
# 2. 查询可加载内核模块
find /lib/modules/$(uname -r) -type f -name '*.ko*'
# 3. 加载内核模块
insmod  /lib/modules/4.18.0-305.3.1.el8.x86_64/extra/openvswitch.ko
# 4. 展示内核模块
lsmod | grep openvswitch 

```