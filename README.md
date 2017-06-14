# BNB		
#### C++大项目设计
----------

## 参与人员及分工
- #### 张文喆
负责游戏架构和网络通信

- #### 李向真
负责UI设计

- #### 林朝坤
负责视觉效果设计

- #### 韩昊玥
负责地图和音乐设计

## 实现的功能
- #### 基础功能
	- [x] 支持地图绘制、人物绘制、水泡效果绘制等
	- [x] 支持鼠标和键盘操作交互
	- [x] 支持障碍物
	- [x] 支持泡泡的放置与爆炸
	- [x] 支持三种基本增强型道具（鞋子，泡泡，药水）
	- [x] 实现服务端：支持局域网联机对战（自由对抗模式），且支持多人在同一个地图游戏
	- [x] 支持动画效果
- #### 拓展功能
	- [x] 支持房间列表
	- [x] 支持 >= 2 张地图
	- [x] 临终礼物

## 用到的C++特性
- #### 基础特性
	- [x] STL 容器，如 std::vector
	- [x] 迭代器
	- [x] 类和多态
- #### C++11特性
	- [x] 初始化列表
	- [x] 类型推断
	- [x] Lambda 表达式

## 游戏系统介绍

### 游戏架构

- #### 地图
    
    - 图形化描述
    ```
    graph TB
    A[地图]-->B[障碍物]
    A-->C[道路]
    B-->D[道具块]
    B-->E[固定块]
    ```
    
    - 实现细节
    ```c++
    for (int i = 0; i < 15; ++i)
	{
		std::vector<Vec2> map;
		std::vector<int> prop;
		for (int j = 0; j < 15; ++j)
		{
			map.push_back(road->getPositionAt(Vec2(i, j)) 
			+ Vec2(deltaX + 16, deltaY - 480 + 16));

			auto tileGidCol = collidable->getTileGIDAt(Point(i,j));
			auto tileGidIte = _item->getTileGIDAt(Point(i, j));

			if (tileGidCol > 0)
				prop.push_back(1);//1代表该块为碰撞块

			else if (tileGidIte > 0)
				prop.push_back(4);//4代表该块为道具块

			else
				prop.push_back(0);//0代表无障碍物
		}
		_mapCoord.push_back(map);
		_mapProp.push_back(prop);
	}
	```

	- 说明  

	地图的块属性被储存在    
	> std::vector<std::vector<int>> _mapProp;   

    中  
    每个块的位置被储存在
    > std::vector<std::vector<Vec2>> _mapCoord; 
    
    中

    0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
    -|-|-|-|-|-|-|-|-|-|-|-|-|-|-
    0|0|4|4|4|0|0|0|0|0|4|4|4|0|0|
    0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
    0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
    0|0|0|0|0|1|1|1|1|1|0|0|0|0|0|
    0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
    0|0|0|0|0|0|1|0|1|0|0|0|0|0|0|
    0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
    0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
    0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
    0|0|0|1|1|1|0|0|0|1|1|1|0|0|0|
    0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
    0|0|0|0|0|4|4|4|4|4|0|0|0|0|0|
    0|0|0|0|0|0|4|4|4|0|0|0|0|0|0|
    0|0|0|0|0|0|0|4|0|0|0|0|0|0|0|

- #### 玩家操作的人物：Player类

    - 图形化描述
    ```
    graph TB
    A[Player]-->B[移动]
    A-->C[动画]
    A-->D[人物属性]
    B-->E[描述移动方向的参数]
    D-->F[人物自身属性]
    D-->G[炸弹相关属性]
    
    ```
    - 人物实体  
    
        ![image](https://github.com/Tongji-Team/BNB/blob/master/Resources/image/player.png)
        
        使用精灵帧中的图片实现
    
    - 人物移动
    
        以“瞬移”的形式移动  
        移动的距离为速度的大小，单位是像素点    
        移动的同时播放动画
        
    - 碰撞
    
        将player图片的四个角设置为碰撞点，则
        
        碰撞点检查 | 左上 | 左下 | 右上 | 右下
        ---|---|---|---|---|
         向左移动| O | O | X | X |
         向右移动| X | X | O | O |
         向上移动| O | X | O | X |
         向下移动| X | O | X | O |
    
        由碰撞点的坐标计算出碰撞点所处的块  
        再由块的属性决定是否发生了碰撞
        
    - 动画  
        
        人物向A方向移动
        
        - 若向A方向移动的动画未播放，则播放此动画
        - 若向A方向移动的动画正在播放，则不做操作
        - 向A方向移动结束后，停止向A方向移动的动画
        
    - 人物属性
    
        ```
        graph TB
        A[人物自身属性]-->B[移动速度]
        A-->C[是否存活]
        ```
        ```
        graph TB
        A[炸弹相关属性]-->B[炸弹数量上限]
        A-->C[当前炸弹数量]
        A-->D[炸弹威力]
        ```
        
- #### 优势的积累：Item类
    
    - 种类
        - 增加移动速度
        - 增加炸弹数量上限
        - 增加炸弹威力
        
    - 道具位置  
    
        藏在道具块下  
        数量一定，位置随机，由服务端产生的随机数种子确定

- #### 杀人（唯一）利器：Bomb类

    - 性质
        - 属于障碍物
        - 安放两秒后爆炸
        - 爆炸产生的波持续0.6秒
        - Player碰到波后将会死亡
        - 有些小技巧
        
    - 碰撞
    ```c++
    bool MainScene::onContactBegin(const PhysicsContact& contact)
    {
    	auto objA = 
    	static_cast<Sprite*>(contact.getShapeA()->getBody()->getNode());
    	auto objB = 
    	static_cast<Sprite*>(contact.getShapeB()->getBody()->getNode());
    	auto nameA = objA->getName();
    	auto nameB = objB->getName();
    	
    	if (nameA == "player"&&nameB == "bomb")
    	{
    		log("player has been slayed");
    		auto player = static_cast<Player*>(objA);
    		player->_isAlive = false;
    	}
    
    	else if (nameA == "bomb"&&nameB == "player")
    	{
    		log("player has been slayed");
    		auto player = static_cast<Player*>(objB);
    		player->_isAlive = false;
    	}
    
    	return true;
    }
    ```
    借助物理碰撞的事件监听器，发生碰撞后，若是一方为波，一方为Player，则判定Player一方被消灭
    
- #### 舞台：MainScene类

    - 加载项
        - 地图
        - Player
        - Item
    - 监听器
        - 键盘监听器
        - 碰撞监听器
    - 交互
        - 人物移动
        - 安放炸弹
        - 炸弹爆炸
        - 胜负判定
    - 网络通信
        - 服务端
        - 客户端（发射器）
        - 客户端（接收器）
        
### 网络通信

**使用了boost库进行多线程(thread)运行和网络通信(asio)**

- #### 技术验证
    https://github.com/zwz1551719/boost-asio-demo

- #### 房间选择界面的通信

    - 通信协议  
        **UDP**
        
    - 数据处理  
        同步

    - 规则  
        - 创建房间的计算机作为服务器运行 
        - 每个计算机都作为客户端运行
        - 服务器进行局域网广播  
        > room: 123, player: 0, map: 0, mapSeed: 32
        
        - 客户端接收广播，读取信息
        - 客户端请求连接
        > connect
        
        - 服务器收到消息，回复客户端并分配Player
        > successfully connected! player1
        
- #### 游戏界面的通信

    - 通信协议  
        **UDP**
        
    - 数据处理  
        同步

    - 规则  
        - 当本机人物位置发生变化或者安放炸弹时，客户端发出消息
        > p2 u l b [123.45,123.45]
        
        - 服务端接收消息，并向其他连接的客户端转发
        - 客户端收到消息，解析消息，更新Player状态
        - 游戏中的逻辑判断发生在客户端本地而非服务端
        
- #### 技术细节

```c++
class Room : public cocos2d::Layer
{
public:
    //...
    void static initBroadcast(Room* ptr);
    void static initClient(Room* ptr);
    void static initReceiver(Room* ptr);
    //...
}
```
类中的成员函数无法直接作为线程函数的参数，是因为成员函数隐含this指针，不符合调用函数的参数要求    
所以需要将这些用于多线程的成员函数声明为static  
而为了使这些函数能够处理类中的数据，必须将类的指针作为参数传入  
> _threadGroup.create_thread(std::bind(&initBroadcast, this));

