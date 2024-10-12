#ifndef SCENARY_H
#define SCENARY_H

#include <Types.h>
#include <yaml-cpp/yaml.h>
namespace Scnry {
// Add custom node types here
enum class UIType{
    BTN,
    TEXT,
    ICON
};
enum class NodeType
{
    EMPTY = 0,
    SKELETAL_MESH = 1,
    STATIC_MESH = 2,
    RIGID_BODY = 3,
    CAMERA = 4,
    VOLUME = 5,
    SCRIPT = 6,
    PARTICLE_EFFECT = 7,
    UI = 8,
    NUM_NODE_TYPES = 9
};
enum class LoadState{
    SUCCESS,
    FAIL
};



class Node;
class Scene{
    public:
    list<Node> SceneNodes; 
    
    Scene(string SceneName = "SCEmpty");
   
    string Name;
    int VersionMaj;
    int VersionMin;
    uint Index;
    float LastEdit;
    
};

//dictionary (list) for each node type to pass a loader callback
class Scnry
{
    public:
        static list<string> Scenes;
        static Scene CurrentLoaded;
        //LoaderFunctions
        static dictionary<NodeType , Node(*)()> NodeLoaders;
        static YAML::Node CurrentLoadedRoot;
};


// Every scene node inherits from this class 
class Node
{
public :
    string Name;
    NodeType Type;
    int Parent;
    Array<float, 16> TransformMatrix;
    public:
        Node();
        Node(const char* name, NodeType&& type, Array<float,16>&& nodeMatrix_16);
        Node(string&& name, NodeType&& type, Array<float,16>&& nodeMatrix_16);
        bool operator ==(const Node& other);
        bool operator !=(const Node& other);
};
class ISerializable{ 
    public :
        virtual string& GetItemPath() const = 0;
        virtual Node& GetItemTransform() const = 0;
};

LoadState LoadScene(const char* ScenePath);
//LoadState LoadScene(uint SceneIndex);
LoadState SaveScene(const char* SaveToPath = "SAME");
//LoadState LoadSceneAsync(const char* ScenePath);

//LoadState AddItemToScene(ISerializable const& Item);
//LoadState RemoveItemFromScene(ISerializable const& Item);

};
#endif