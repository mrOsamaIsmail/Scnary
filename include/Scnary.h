#ifndef SCENARY_H
#define SCENARY_H

#include <Types.h>

enum class LoadState{
    SUCCESS,
    FAIL
};

class Scene{
    public:
    Scene(string SceneName = "SCEmpty"):
    Name(SceneName),
    Index(0)
    {}
        string Name;
        uint Index;
};
class Scnary
{
    public:
        static list<string> Scenes;
        static Scene* CurrentLoaded;
};

class Transform;
class ISerializable{ 
    public :
        virtual string& GetItemPath() const = 0;
        virtual Transform& GetItemTransform() const = 0;
};

LoadState LoadScene(const char* ScenePath);
LoadState LoadScene(uint SceneIndex);
LoadState SaveScene(const char* SaveToPath = "SAME");
LoadState LoadSceneAsync(const char* ScenePath);

LoadState AddItemToScene(ISerializable const& Item);
LoadState RemoveItemFromScene(ISerializable const& Item);
#endif