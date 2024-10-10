#include <Scnry.h>
#include <yaml-cpp/yaml.h>

//delete me
#include <iostream>

//list<string> Scnry::Scnry::Scenes = list<string>();
/*dictionary<NodeType , Scnry::Node(*)(string const&)>*/ //Scnry::Scnry = dictionary<NodeType , Scnry::Node(*)(string const&)>();
Scnry::Scene* Scnry::Scnry::CurrentLoaded = nullptr;

Scnry::Node::Node(const char* name, NodeType&& type, Array<float,16>&& nodeMatrix_16)
:
    Name(string(name)),
    Type(type),
    TransformMatrix(nodeMatrix_16)
{}
Scnry::Node::Node(string&& name, NodeType&& type, Array<float,16>&& nodeMatrix_16)
:
    Name(string(name)),
    Type(type),
    TransformMatrix(nodeMatrix_16)
{}

LoadState Scnry::LoadScene(const char* ScenePath)
{
    YAML::Node file = YAML::LoadFile(ScenePath);
    if(!file["SceneNodes"])
        return LoadState::FAIL;
    YAML::Node sceneNodes = YAML::Load("Node");
    for (std::size_t i=0;i<sceneNodes.size();i++) {
        std::cout << sceneNodes[i].as<string>() << "\n";
    }
    
}
/*
LoadState LoadScene(uint SceneIndex)
{

    return LoadState::SUCCESS;
}
LoadState SaveScene(const char* SaveToPath)
{

    return LoadState::SUCCESS;
}
LoadState LoadSceneAsync(const char* ScenePath)
{
    return LoadState::SUCCESS;
}

LoadState AddItemToScene(Scnry::ISerializable const& Item)
{

    return LoadState::SUCCESS;
}
LoadState RemoveItemFromScene(Scnry::ISerializable const& Item)
{

    return LoadState::SUCCESS;
}*/