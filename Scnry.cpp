
#include <yaml-cpp/yaml.h>
#include <Scnry.hpp>
#include <fstream>
#include <chrono>

class Scnry::Impl
{
public:
    YAML::Node Node;
    Impl() {}

    operator YAML::Node()const
    {
        return this->Node;
    }

    void SetNode(const YAML::Node& node)
    {
        this->Node = node;
    }
    //definitions are in the bottom

};

namespace YAML
{
    //template <typename T>
    //struct convert<std::shared_ptr<T>> {
    //    static Node encode(const std::shared_ptr<T>& ptr) {
    //        if (!ptr) return Node();  // Handle nullptr case

    //        return convert<T>::encode(*ptr);  // Encode the object itself
    //    }

    //    static bool decode(const Node& node, std::shared_ptr<T>& ptr) {
    //        if (!node.IsDefined()) return false;

    //        T obj;
    //        bool success = convert<T>::decode(node, obj);
    //        if (success) {
    //            ptr = std::make_shared<T>(obj);
    //        }
    //        return success;
    //    }
    //};
    template <>
    struct convert<Scnry::Node> {
        static Node encode(const Scnry::Node& node)
        {
            Node _node;

            _node.push_back("Name"); //,node.Name);
            _node.push_back("Parent"); //,node.Name);
            _node.push_back("NodeType"); //,node.Name);
            _node.push_back("Transform"); //,node.Name);
            
            return _node;
        }
        static bool decode(const Node& node, Scnry::Node& SceneNode)
        {
            SceneNode.Name = node["Name"].as<string>();
            SceneNode.Parent = node["Parent"].as<int>();
            SceneNode.Type = static_cast<Scnry::NodeType>(node["NodeType"].as<int>());
            SceneNode.TransformMatrix = node["Transform"].as<std::array<float, 16>>();
            return true;
        }


    };
    template<>
    struct convert<std::shared_ptr<Scnry::Impl>> {
        static Node encode(std::shared_ptr<Scnry::Impl> impl)
        {
            Node _node;

            _node.push_back("Name"); //,node.Name);
            _node.push_back("Parent"); //,node.Name);
            _node.push_back("NodeType"); //,node.Name);
            _node.push_back("Transform"); //,node.Name);

            return _node;
        }
        static bool decode(const Node& node, std::shared_ptr<Scnry::Impl>& impl)
        {
            impl->Node = node;

            /*SceneNode.Parent = node["Parent"].as<int>();
            SceneNode.Type = static_cast<Scnry::NodeType>(node["NodeType"].as<int>());
            SceneNode.TransformMatrix = node["Transform"].as<std::array<float, 16>>();*/
            return true;
        }


    };

}




std::shared_ptr<Scnry::Impl> Scnry::Scnry::implementation;
dictionary<Scnry::NodeType, bool(*)(Scnry::Node&,const string&)> Scnry::Scnry::NodeLoaders;
bool Scnry::Scnry::Init() 
{
    //Scnry::Scnry::NodeLoaders() //= dictionary<Scnry::NodeType, bool(*)(Scnry::Node&, const string&)>();
    Scnry::Scnry::CurrentLoaded = Scene();
    Scnry::implementation = std::make_shared<Impl>();
    return true;
}

Scnry::Scene Scnry::Scnry::CurrentLoaded;

Scnry::Scene::Scene(string SceneName) :
    Name(SceneName),
    SceneNodes(),
    Index(0),
    VersionMaj(0),
    VersionMin(0),
    LastEdit(0.0f)
{}
Scnry::Node::Node(const char* name, NodeType&& type, Array<float,16>&& nodeMatrix_16)
:
    Name(string(name)),
    Type(type),
    TransformMatrix(nodeMatrix_16),
    Parent(-1)
{}
Scnry::Node::Node(string&& name, NodeType&& type, Array<float,16>&& nodeMatrix_16)
:
    Name(string(name)),
    Type(type),
    TransformMatrix(nodeMatrix_16),
    Parent(-1)
{}
Scnry::Node::Node():Name(""),Type(NodeType::EMPTY),TransformMatrix(),Parent(-1)
{}
bool Scnry::Node::operator ==(const Node& other) 
{
    if (other.Name == Name && other.Parent == Parent && other.Type == Type && other.TransformMatrix == TransformMatrix)
        return true;
    else
        return false;
}
bool Scnry::Node::operator !=(const Node& other) 
{
    if (other.Name == Name && other.Parent == Parent && other.Type == Type && other.TransformMatrix == TransformMatrix)
        return false;
    else
        return true;
}

Scnry::LoadState Scnry::LoadScene(const char* ScenePath)
{
    string path = string(ScenePath);
    
    YAML::Node file = YAML::LoadFile(path);
    Scene currentScene;
    currentScene.Name = file["SceneName"].as<string>();
    currentScene.VersionMaj = (file["Version"])["Maj"].as<int>();
    currentScene.VersionMin = (file["Version"])["Min"].as<int>();
  
        
    YAML::Node& sceneNodes = file["SceneNodes"];
    sceneNodes.SetStyle(YAML::EmitterStyle::Block);
    Scnry::implementation->SetNode(file);
    
    for (size_t i = 0; i < sceneNodes.size(); i++)
    {
        YAML::Node& node = sceneNodes[i];
         
        std::string res;
        
        Node Scene_node = Node();
        Scene_node.Name = node["Name"].as<string>();
        Scene_node.Type = static_cast<NodeType>(node["NodeType"].as<int>());
        Scene_node.Parent = node["Parent"].as<int>();
        Scene_node.TransformMatrix = node["Transform"].as<Array<float,16>>();

        Scnry::Scnry::NodeLoaders[Scene_node.Type](Scene_node,node["AssetId"].as<string>());
        
    }
    Scnry::CurrentLoaded = currentScene;
    Scnry::implementation->SetNode(file);
    return LoadState::SUCCESS;
}

Scnry::LoadState Scnry::Scnry::LoadScene(uint SceneIndex)
{
    //Scnry::implementation = new Impl();
    return LoadState::SUCCESS;
}
Scnry::LoadState Scnry::SaveScene(const char* SaveToPath)
{
    YAML::Node node;
    node["SceneName"] = Scnry::CurrentLoaded.Name;
    node["VersionMaj"] = Scnry::CurrentLoaded.VersionMaj;
    node["VersionMin"] = Scnry::CurrentLoaded.VersionMin;
    YAML::Emitter out;

    out << (YAML::Node)Scnry::Scnry::implementation;

    if (!out.good()) {
#ifdef DEBUG

        std::cerr << "YAML Emission failed: " << out.GetLastError() << "\n";
#endif // DEBUG

        return LoadState::FAIL;
    }

    std::ofstream fileOut(SaveToPath);
    fileOut << out.c_str();
    fileOut.close();
    return LoadState::SUCCESS;
}
Scnry::LoadState Scnry::Scnry::LoadSceneAsync(const char* ScenePath)
{
    return LoadState::SUCCESS;
}

Scnry::LoadState Scnry::Scnry::AddItemToScene(ISerializable const& Item)
{
    YAML::Node newNode;
    newNode["Name"] = "NewNode";
    newNode["Transform"] = std::array<float, 16>({ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
    newNode["Transform"].SetStyle(YAML::EmitterStyle::Flow);
    newNode["Parent"] = 2;
    newNode["NodeType"] = 6;
    newNode["AssetId"] = string("FFXG");

    ((YAML::Node)Scnry::Scnry::implementation)["SceneNodes"].push_back(newNode);
    //auto& nodesNode = file["SceneNodes"];
    return LoadState::SUCCESS;
}

Scnry::LoadState Scnry::Scnry::RemoveItemFromScene(ISerializable const& Item)
{

    return LoadState::SUCCESS;
}
