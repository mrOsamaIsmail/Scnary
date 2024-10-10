#include <Scnry.h>




#include <iostream>
#include <string>

int main()
{
   
    //Scnry::LoadScene("tempScene.yaml");
    std::string path = ("tempScene.yaml");
    /*YAML::Node config = */ Scnry::LoadScene("tempScene.yaml");
    return 0;
}


