# RPG_Spldn

基于 **Unreal Engine 5.6** 的 ARPG 战斗系统项目，使用 **Gameplay Ability System (GAS)** 框架实现角色技能、属性、Buff/Effect 和战斗逻辑。

## 技术栈

| 模块/插件 | 用途 |
|---|---|
| `GameplayAbilities` | 技能系统核心 (GAS) |
| `GameplayTags` | 技能标签、事件标签管理 |
| `GameplayTasks` | 异步技能任务 |
| `EnhancedInput` | 增强输入系统 |
| `UMG` | UI 控件 |
| `AIModule` | 敌人 AI 行为 |

## 项目结构

```
Source/ChaPanda/
├── Public/
│   ├── AbilitySystem/
│   │   ├── Abilities/
│   │   │   ├── CP_GameplayAbility.h          # 技能基类
│   │   │   ├── Player/
│   │   │   │   └── CP_Primary.h              # 玩家普攻技能
│   │   │   └── Enemy/
│   │   │       ├── CP_HitReact.h             # 受击反应技能
│   │   │       └── CP_SearchForTarget.h      # 敌人索敌攻击技能
│   │   ├── AbilityTasks/
│   │   │   └── CP_WaitGameplayEvent.h        # 异步等待 GameplayEvent
│   │   ├── CP_AbilitySystemComponent.h       # 自定义 ASC
│   │   └── CP_AttributeSet.h                 # 属性集 (Health/Mana)
│   ├── Character/
│   │   ├── CP_BaseCharacter.h                # 角色基类 (实现 IAbilitySystemInterface)
│   │   ├── CP_PlayerCharacter.h              # 玩家角色
│   │   └── CP_EnemyCharacter.h               # 敌人角色
│   ├── Player/
│   │   ├── CP_PlayerController.h             # 玩家控制器 (EnhancedInput)
│   │   └── CP_PlayerState.h                  # 玩家状态 (持有 ASC)
│   ├── GameObject/
│   │   └── CP_Projectile.h                   # 弹射物
│   ├── Notifies/
│   │   └── CP_MeleeAttack.h                  # 动画通知状态 - 近战攻击检测
│   ├── UI/
│   │   ├── CP_AttributeWidget.h              # 属性绑定 Widget 基类
│   │   └── CP_WidgetComponent.h              # 自动绑定 ASC 的 WidgetComponent
│   ├── Tasks/
│   │   └── CP_AttributeChangeTask.h          # 蓝图异步任务 - 监听属性变化
│   ├── Utils/
│   │   └── CP_BlueprintFunctionLibrary.h     # 蓝图函数库 (伤害/击退/命中检测)
│   └── GameplayTags/
│       └── CP_Tags.h                         # Native Gameplay 标签定义
└── Private/                                  # 对应 .cpp 实现文件
```

## 架构设计

### 角色继承体系

```
ACharacter
  └── ACP_BaseCharacter (IAbilitySystemInterface)   ← 抽象基类
        ├── ACP_PlayerCharacter                      ← 玩家角色 (ASC 在 PlayerState)
        └── ACP_EnemyCharacter                       ← 敌人角色 (ASC 在自身)
```

- **玩家**：ASC 和 AttributeSet 由 `ACP_PlayerState` 持有，通过网络复制同步
- **敌人**：ASC 和 AttributeSet 由 `ACP_EnemyCharacter` 自身持有

### GAS 核心组件

| 组件 | 类 | 职责 |
|---|---|---|
| **ASC** | `UCP_AbilitySystemComponent` | 技能授予、激活、等级管理 |
| **AttributeSet** | `UCP_AttributeSet` | `Health` / `MaxHealth` / `Mana` / `MaxMana`，支持网络复制 |
| **GameplayAbility** | `UCP_GameplayAbility` | 技能基类，内建调试绘制开关 |

### 技能系统

```
UCP_GameplayAbility (继承 UGameplayAbility)
  ├── UCP_Primary          # 玩家普攻：HitBox 重叠检测 → 发送 HitReact 事件
  ├── UCP_SearchForTarget  # 敌人 AI：搜索 → 移动 → 攻击 循环 (AITask_MoveTo)
  └── UCP_HitReact         # 受击反应：计算攻击方向 (前/后/左/右)
```

### 战斗流程

```
攻击动画播放
  → UCP_MeleeAttack (AnimNotifyState) 逐帧 SphereTrace
    → 命中敌人 → 发送 MeleeTraceHit GameplayEvent
      → 敌人收到 HitReact 事件
        → UCP_HitReact 计算受击方向 → 播放对应蒙太奇
      → 施加伤害 GE (GameplayEffect)
```

### 输入系统

`ACP_PlayerController` 使用 **EnhancedInput**：
- `MoveAction` / `LookAction` / `JumpAction` — 移动/视角/跳跃
- `PrimaryAction` / `SecondaryAction` / `TertiaryAction` — 三个技能槽位，通过 GameplayTag 激活对应技能

### 蓝图异步任务

- **`UCP_AttributeChangeTask`**：将 ASC 的属性变化委托封装为蓝图异步节点 (`ListenForAttributeChange`)，在蓝图中通过事件驱动方式响应属性变化
- **`UCP_WaitGameplayEvent`**：扩展 `UAbilityAsync_WaitGameplayEvent`，支持按 Actor 代理等待 GameplayTag 事件

### UI 架构

- `UCP_AttributeWidget` — 绑定一对属性 (当前值/最大值) 的 Widget 基类，通过 `BP_OnAttributeChanged` 驱动蓝图更新
- `UCP_WidgetComponent` — 附加在角色上的 3D Widget，自动检测角色 ASC 初始化 → 绑定属性变化委托 → 传递到子 Widget

### 工具库

`UCP_BlueprintFunctionLibrary` 提供：
- `GetHitDirection` — 基于攻击者方向计算受击方向枚举
- `FindClosestActorWithTag` — 按 Tag 搜索最近 Actor (范围滤)
- `SendDamageEventToPlayer(s)` — 向目标施加伤害 GE 并附带 GameplayEvent
- `HotBoxOverlapTest` — 圆柱体碰撞检测 (可配置半径/前向偏移/高度)
- `ApplyKnockback` — 径向击退 (内圈全力/外圈衰减)

## GameplayTag 命名空间

| 命名空间 | 用途 |
|---|---|
| `CPAbilities.*` | 技能激活标签 (Primary/Secondary/Tertiary/Enemy.Attack) |
| `CPAbilities.ActivateOnGiven` | 授予时自动激活 |
| `Events.Player.*` | 玩家事件 (HitReact/Death) |
| `Events.Enemy.*` | 敌人事件 (HitReact/EndAttack/MeleeTraceHit) |
| `Events.KillScored` | 击杀事件 |
| `SetByCaller.Projectile` | 弹射物伤害数值 |


**引擎版本**: Unreal Engine 5.6
