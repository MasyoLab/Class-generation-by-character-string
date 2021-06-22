#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define _GLIBCXX_DEBUG
#include <iostream>
#include <unordered_map>
#include <string>

//==========================================================================
// サンプルクラス
class IObject // 継承用
{
public:
    IObject() {}
    ~IObject() {}
    virtual void Text() = 0;
private:
};

class Object1 : public IObject // 1
{
public:
    Object1() {}
    ~Object1() {}
    void Text() override { std::cout << "Object1" << std::endl; }
private:
};

class Object2 : public IObject // 2
{
public:
    Object2() {}
    ~Object2() {}
    void Text() override { std::cout << "Object2" << std::endl; }
private:
};

class Object3 : public IObject // 3
{
public:
    Object3() {}
    ~Object3() {}
    void Text() override { std::cout << "Object3" << std::endl; }
private:
};

// サンプルクラス
//==========================================================================

class IObjectBuilder // 生成機能の基礎
{
public:
    IObjectBuilder() {};
    virtual ~IObjectBuilder() {};
    virtual IObject* Create() = 0;
};

template<typename _Ty>
class ObjectBuilder : public IObjectBuilder  // 生成機能
{
public:
    ObjectBuilder() {};
    ~ObjectBuilder() {};
    _Ty* Create() { return new _Ty; }
};

// 生成に使うマクロ
// クラス名の文字列変換も行います
#define REGISTER_ARGS(TYPE) #TYPE,new ObjectBuilder<TYPE>

// 生成対象のクラスを管理する
class BuilderFromString
{
public:
    BuilderFromString() {}
    ~BuilderFromString() {
        for (auto & itr : builders) {
            delete itr.second;
        }
        builders.clear();
    }

    // 生成対象を登録
    void Register(const std::string& ty_name, IObjectBuilder* builder) {
        builders.insert(std::make_pair(ty_name, builder));
    }

    // クラス名を指定して生成
    IObject* Create(const std::string& ty_name) {
        auto itr = builders.find(ty_name);
        if (itr == builders.end())return nullptr;
        return (*itr).second->Create();
    }
private:
    std::unordered_map<std::string, IObjectBuilder*> builders;
};

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 使用例
    BuilderFromString builderfromstring;

    // 登録
    builderfromstring.Register(REGISTER_ARGS(Object1));
    builderfromstring.Register(REGISTER_ARGS(Object2));
    builderfromstring.Register(REGISTER_ARGS(Object3));

    // 生成
    auto* object1 = builderfromstring.Create("Object1");
    auto* object2 = builderfromstring.Create("Object2");
    auto* object3 = builderfromstring.Create("Object3");

    // テスト用描画
    object1->Text();
    object2->Text();
    object3->Text();

    // 作られた実体は破棄
    delete object1;
    delete object2;
    delete object3;

    return std::system("PAUSE");
}