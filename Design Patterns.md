# 创建型模式

## ABSTRACT FACTORY

提供一个创建一系列相关或互相依赖对象的接口，而无需指定它们具体的类。通常是一个Singleton。

```c++
class MazeFactory {
public:
  MazeFactory();
  
  virtual Maze* MakeMaze() const { return new Maze; }
  virtual Wall* MakeWall() const { return new Wall; }
  virtual Room* MakeRoom(int n) const { return new Room(n); }
  ...
};

class MazeA : public MazeFactory {
public:
  MazeA();
  
  Room* MakeRoom(int n) const { return new MazeARoom(n); }
  ...
};

class MazeB : public MazeFactory { ... };

```

## BUILDER

将一个复杂对象的构造与它的表示分离，使得同样的构造过程可以创建不同的表示。

Builder,Concrete Builder,Director,Product

```c++
```