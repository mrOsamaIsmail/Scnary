#include <Scnry.hpp>



#define PRINT(x) std::cout << x << std::endl;
#include <iostream>
#include <string>


bool MeshLoader(Scnry::Node& node,const string& str) 
{
    std::cout << "Loading a mesh" << "\t";
    std::cout << node.Name << "\n";
    return true;
}

bool CameraLoader(Scnry::Node& node,const string& str)
{
    std::cout << "Loading a Camera" << "\t";
    std::cout << node.Name << "\n";
    return true;
}
int main()
{
   
    //Scnry::LoadScene("tempScene.yaml");
    Scnry::Scnry::Init();
    Scnry::Scnry::NodeLoaders[Scnry::NodeType::STATIC_MESH] = MeshLoader;
    Scnry::Scnry::NodeLoaders[Scnry::NodeType::CAMERA] = CameraLoader;
    std::string path = ("tempScene.scene");
    Scnry::Scnry::LoadScene("./tempScene.scene");
    PRINT(Scnry::Scnry::CurrentLoaded.Name);
    PRINT(Scnry::Scnry::CurrentLoaded.SceneNodes.size());
    Scnry::Scnry::SaveScene("./tempScene.scene");
    return 0;
}


