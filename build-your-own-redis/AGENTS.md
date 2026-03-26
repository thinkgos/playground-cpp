# AGENTS.md

Agent 在此代码库中工作的快速指南。

## 重要提示

- **源代码位置**：源代码在项目根目录、`container/`、`server/`、`client/` 中。`build/` 目录是编译产物，除非必要（如检查测试输出），请避免读取。
- **工作流管理**：本项目使用 **go-task** (`Taskfile.yml`) 管理构建、测试、运行。优先使用 `task` 命令，而不是手动执行 CMake 步骤。

## 项目简介

基于 ["Build Your Own Redis"](https://build-your-own.org/redis) 的类 Redis 内存键值存储，包含三个组件：

1. **容器库** (`container/`) – 哈希表、AVL树、堆、LRU缓存、链表、线程池。
2. **服务器** (`server/`) – 单线程事件循环，支持 Redis 命令子集（`get`、`set`、`del`、`pexpire`、`pttl`、`keys`、`zadd`、`zrem`、`zscore`、`zquery`）。
3. **客户端** (`client/`) – 命令行客户端。

## 使用 go-task 快速参考

| 任务 | 命令 | 说明 |
|------|------|------|
| 默认运行 | `task` | 执行 `task run` |
| 清理 | `task clean` | 删除 `build/` 和 `install/` |
| 配置 | `task configure` | 需要 `PRESET` 变量（例如 `PRESET=redis`） |
| 构建 | `task build` | 需要 `PRESET` 变量，可选 `TARGET` |
| 安装 | `task install` | 安装到 `install/` |
| 运行测试 | `task tests` | 需要 `PRESET` 变量，可选 `TAG` 过滤 |
| 一键运行 | `task run` | 配置、构建、安装（默认 `PRESET=redis`） |
| 一键测试 | `task run:tests` | 配置、构建、运行单元测试 |

**常用预设**：
- `redis`：标准构建预设（参见 `CMakePresets.json`）。

**查看所有任务**：
```bash
task --list
```

## 常用命令示例

```bash
# 构建并运行所有测试
task run:tests

# 启动服务器（先构建）
task run

# 手动运行服务器（在 build/ 目录下）
./build/server/server

# 使用客户端
./build/client/client set foo bar
./build/client/client get foo

# 检查内存泄漏（调试构建）
cd build && ASAN_OPTIONS=detect_leaks=1 ./server/server
```

## 开发注意事项（精简）

- **C++20**：代码使用 C++20。
- **调试**：调试构建已启用 AddressSanitizer、LeakSanitizer、UndefinedBehaviorSanitizer。
- **代码风格**：中英文注释混合；偏好侵入式数据结构以提升性能。
- **添加新命令**：
  1. 在 `server/main.cc` 的 `do_request()` 中添加解析。
  2. 实现 `do_*` 函数，接收 `std::vector<std::string>& cmd` 和 `Buffer& out`。
  3. 客户端自动发送任意命令行参数，通常无需修改客户端。
- **测试**：单元测试位于 `container/` 目录下的 `*_test.cxx` 文件中，使用 GTest。
- **内存管理**：对象由全局 `g_data` 拥有，显式删除；大型容器通过线程池异步删除。

## 快速开始

1. 确保已安装 [go-task](https://taskfile.dev/installation/)。
2. 运行 `task run:tests` 构建并测试项目。
3. 运行 `task run` 启动服务器，然后使用客户端连接。

> 更详细的架构、协议等信息请参阅代码注释和原始教程。