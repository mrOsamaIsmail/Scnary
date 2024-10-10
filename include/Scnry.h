#ifndef SCENARY_H
#define SCENARY_H

#include <Types.h>


// Add custom node types here
enum class UIType{
    BTN,
    TEXT,
    ICON
};
enum class NodeType
{
    EMPTY,
    SKELETAL_MESH,
    STATIC_MESH,
    RIGID_BODY,
    CAMERA,
    VOLUME,
    SCRIPT,
    PARTICLE_EFFECT,
    UI,
    NUM_NODE_TYPES
};
enum class LoadState{
    SUCCESS,
    FAIL
};

namespace Scnry{

class Node;
class Scene{
    public:
    list<Node> SceneNodes; 
    Scene(string SceneName = "SCEmpty"):
    Name(SceneName),
    SceneNodes(),
    Index(0)
    {}
        string Name;
        uint Index;
};

//dictionary (list) for each node type to pass a loader callback
class Scnry
{
    public:
        static list<string> Scenes;
        static Scene* CurrentLoaded;
        //LoaderFunctions
        static dictionary<NodeType , Node(*)(string const&)> NodeLoaders;
};


// Every scene node inherits from this class 
class Node
{
    string Name;
    NodeType Type;
    Node* Parent;
    Array<float, 16> TransformMatrix;
    public:
        Node(const char* name, NodeType&& type, Array<float,16>&& nodeMatrix_16);
        Node(string&& name, NodeType&& type, Array<float,16>&& nodeMatrix_16);
};
class ISerializable{ 
    public :
        virtual string& GetItemPath() const = 0;
        virtual Node& GetItemTransform() const = 0;
};

LoadState LoadScene(const char* ScenePath);
//LoadState LoadScene(uint SceneIndex);
//LoadState SaveScene(const char* SaveToPath = "SAME");
//LoadState LoadSceneAsync(const char* ScenePath);

//LoadState AddItemToScene(ISerializable const& Item);
//LoadState RemoveItemFromScene(ISerializable const& Item);

};
#endif