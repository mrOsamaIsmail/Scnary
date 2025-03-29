
#include <yaml-cpp/yaml.h>
#include <Scnry.hpp>
#include <fstream>
#include <chrono>

class Scnry::Impl
{
public:
    //YAML::Node Node;
    Impl() {}

    /*operator YAML::Node()const
    {
        return this->Node;
    }*/

   /* void SetNode(const YAML::Node& node)
    {
        this->Node = node;
    }*/
    //definitions are in the bottom

};

namespace YAML
{
    /*template <>
    struct convert<Scnry::NodeType> 
    {
        static Node encode(const Scnry::NodeType& ntype) 
        {
            Node nod;
            nod["Type"] = (int)ntype;
            return nod;
        }
        static bool decode(const Node& node, Scnry::NodeType& dest) 
        {
            dest = static_cast<Scnry::NodeType>(node["Type"].as<int>());
        }
    };*/
    template<>
    struct convert<Scnry::Version> 
    {
        static Node encode(const Scnry::Version& input) 
        {
            Node node;
            node["Maj"] = input.maj;
            node["Min"] = input.min;
            return node;
        }
        static bool decode(const Node& node, Scnry::Version& output) 
        {
            output.maj = node["Maj"].as<int>();
            output.min = node["Min"].as<int>();
            return true;
        }
    };
    template <>
    struct convert<Scnry::Node> {
        static Node encode(const Scnry::Node& node)
        {
            Node _node;
            _node["Name"] = node.Name;
            _node["Parent"] = node.Parent;
            _node["NodeType"] = (int)node.Type;
            _node["Transform"] = node.TransformMatrix;
            _node["Transform"].SetStyle(YAML::EmitterStyle::Flow);
            _node["AssetID"] = node.AssetID;
            return _node;
        }
        static bool decode(const Node& node, Scnry::Node& SceneNode)
        {
            SceneNode.Name = node["Name"].as<string>();
            SceneNode.Parent = node["Parent"].as<int>();
            SceneNode.Type = static_cast<Scnry::NodeType>(node["NodeType"].as<int>());
            SceneNode.TransformMatrix = node["Transform"].as<std::array<float, 16>>();
            SceneNode.AssetID = node["AssetID"].as<long>();
            return true;
        }


    };
    template<>
    struct convert<Scnry::Scene>
    {
        static Node encode(const Scnry::Scene& scene) 
        {
            Node sceneNode;
            sceneNode["SceneName"] = scene.Name;
            sceneNode["Version"] = scene.Version;
            sceneNode["SceneNodes"] = YAML::Node(YAML::NodeType::Sequence);
            
            for (const auto& node : scene.SceneNodes) {
                sceneNode["SceneNodes"].push_back(node);
            }
            return sceneNode;
        }
    };
}




std::shared_ptr<Scnry::Impl> Scnry::Scnry::implementation;
dictionary<Scnry::NodeType, bool(*)(Scnry::Node&,const string&)> Scnry::Scnry::NodeLoaders;
Scnry::Scene Scnry::Scnry::CurrentLoaded;

bool Scnry::Scnry::Init() 
{
    //Scnry::Scnry::NodeLoaders() //= dictionary<Scnry::NodeType, bool(*)(Scnry::Node&, const string&)>();
    Scnry::Scnry::CurrentLoaded = Scene();
    Scnry::implementation = std::make_shared<Impl>();
    return true;
}

Scnry::Version::Version(int _maj, int _min): maj(maj), min(_min){}
Scnry::Version::Version(): maj(0), min(0){}

Scnry::Scene::Scene(string SceneName) :
    Name(SceneName),
    SceneNodes(),
    Index(0),
    LastEdit(0.0f)
{}
Scnry::Node::Node(const char* name, NodeType&& type, Array<float,16>&& nodeMatrix_16,long ID)
:
    Name(string(name)),
    Type(type),
    TransformMatrix(nodeMatrix_16),
    Parent(-1),
    AssetID(ID)
{}
Scnry::Node::Node(string&& name, NodeType&& type, Array<float,16>&& nodeMatrix_16)
:
    Name(string(name)),
    Type(type),
    TransformMatrix(nodeMatrix_16),
    Parent(-1)
{}
Scnry::Node::Node():
    Name(""),
    Type(NodeType::EMPTY),
    TransformMatrix(),
    Parent(-1)
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

Scnry::LoadState Scnry::Scnry::LoadScene(const char* ScenePath)
{
    string path = string(ScenePath);
    
    YAML::Node file = YAML::LoadFile(path);
    Scene currentScene;
    
    currentScene.Name = file["SceneName"].as<string>();
    currentScene.Version = (file["Version"]).as<Version>();
  
        
    YAML::Node& sceneNodes = file["SceneNodes"];
    sceneNodes.SetStyle(YAML::EmitterStyle::Block);
    //Scnry::implementation->SetNode(file);
    
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
        currentScene.SceneNodes.push_back(Scene_node);
    }
    Scnry::CurrentLoaded = currentScene;
    //Scnry::implementation->SetNode(file);
    return LoadState::SUCCESS;
}

Scnry::LoadState Scnry::Scnry::LoadScene(uint SceneIndex)
{
    //Scnry::implementation = new Impl();
    return LoadState::SUCCESS;
}
Scnry::LoadState Scnry::Scnry::SaveScene(const char* SaveToPath)
{
    YAML::Node node;
    node["Scene"] = Scnry::CurrentLoaded;
    YAML::Emitter out;

    out << node;//(YAML::Node)Scnry::Scnry::implementation;

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

Scnry::LoadState Scnry::Scnry::AddItemToScene(ISerializable const& Item)
{
    YAML::Node newNode;
    newNode["Name"] = "NewNode";
    newNode["Transform"] = std::array<float, 16>({ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
    newNode["Transform"].SetStyle(YAML::EmitterStyle::Flow);
    newNode["Parent"] = 2;
    newNode["NodeType"] = 6;
    newNode["AssetId"] = string("FFXG");

    //((YAML::Node)Scnry::Scnry::implementation)["SceneNodes"].push_back(newNode);
    //auto& nodesNode = file["SceneNodes"];
    return LoadState::SUCCESS;
}

Scnry::LoadState Scnry::Scnry::RemoveItemFromScene(ISerializable const& Item)
{

    return LoadState::SUCCESS;
}
