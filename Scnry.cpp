//dell me plz
#include <iostream>

#include <Scnry.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <chrono>



namespace YAML 
{
    template <>
    struct convert<Scnry::Node> {
        static Node encode(const Scnry::Node& node)
        {
            Node _node;
            /*_node.push_back("Name")//,node.Name);
            _node["Parent"] = node.Parent;
            _node["NodeType"] = node.Type;
            _node["Transform"] = node.TransformMatrix;*/

            _node.push_back("Name"); //,node.Name);
            _node.push_back("Parent"); //,node.Name);
            _node.push_back("NodeType"); //,node.Name);
            _node.push_back("Transform"); //,node.Name);
           /* _node["Name"] = node.Name;
            _node["Parent"] = node.Parent;
            _node["NodeType"] = node.Type;
            _node["Transform"] = node.TransformMatrix; */
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
}
//list<string> Scnry::Scnry::Scenes = list<string>();
dictionary<Scnry::NodeType, Scnry::Node(*)()> Scnry::Scnry::NodeLoaders;

Scnry::Scene Scnry::Scnry::CurrentLoaded;

Scnry::Scene::Scene(string SceneName = "SCEmpty") :
    Name(SceneName),
    SceneNodes(),
    Index(0),
    VersionMaj(0),
    VersionMin(0),
    LastEdit(0)
{}
YAML::Node Scnry::Scnry::CurrentLoadedRoot;
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
    Scnry::CurrentLoaded = currentScene;
    Scnry::CurrentLoadedRoot = file;
        
    YAML::Node& sceneNodes = file["SceneNodes"];
    Scnry::CurrentLoadedRoot = file;
    prntln(sceneNodes.Type());
    for (size_t i = 0; i < sceneNodes.size(); i++)
    {
        YAML::Node& node = sceneNodes[i];
        NodeType type = static_cast<NodeType>(node["NodeType"].as<int>());
        std::string res;
        // -TBD Pass the node to its loader-
        //Scnry::NodeLoaders[type];
    }

    YAML::Node newNode;
    newNode["Name"] = "NewNode";
    newNode["Transform"] = std::array<float, 16>({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    newNode["Parent"] = 2;
    newNode["NodeType"] = 6;

    // Add this new node to SceneNodes
    file["SceneNodes"].push_back(newNode);

    return LoadState::SUCCESS;
}

Scnry::LoadState LoadScene(uint SceneIndex)
{

    return Scnry::LoadState::SUCCESS;
}
Scnry::LoadState Scnry::SaveScene(const char* SaveToPath)
{
    YAML::Node node;
    node["SceneName"] = Scnry::CurrentLoaded.Name;
    node["VersionMaj"] = Scnry::CurrentLoaded.VersionMaj;
    node["VersionMin"] = Scnry::CurrentLoaded.VersionMin;
    YAML::Emitter out;
    out.SetMapFormat(YAML::Flow);
    //out.SetSeqFormat(YAML::BeginSeq);
    out << Scnry::Scnry::CurrentLoadedRoot;

    if (!out.good()) {
        std::cerr << "YAML Emission failed: " << out.GetLastError() << "\n";
        return LoadState::FAIL;
    }


    std::ofstream fileOut(SaveToPath);
    fileOut << out.c_str();
    fileOut.close();
    return LoadState::SUCCESS;
}
Scnry::LoadState LoadSceneAsync(const char* ScenePath)
{
    return Scnry::LoadState::SUCCESS;
}

Scnry::LoadState AddItemToScene(Scnry::ISerializable const& Item)
{

    return Scnry::LoadState::SUCCESS;
}
Scnry::LoadState RemoveItemFromScene(Scnry::ISerializable const& Item)
{

    return Scnry::LoadState::SUCCESS;
}