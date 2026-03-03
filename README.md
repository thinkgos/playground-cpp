# playground-cpp

## 类型箤取(Type Traits)

核心原理: 模板特化与SFINAE

- 模板特化(Template Specialization): 定义一个通用模板, 并为特定类型提供特殊实现.
- SFINAE(Substitution Failure Is Not An Error): 当模板实例化时, 如果替换后的类型不满足要求, 则静默地忽略那些不符合条件的候选者, 而是继续尝试其他模板实例化.
