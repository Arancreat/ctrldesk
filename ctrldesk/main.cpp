#include "cli.h"
#include "render.h"

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);

    vector <string>
        argv_vector,
        path_vector;

    //TODO: Argument parsing and processing 

    for (int i = 0; i < argc; i++)
    {
        argv_vector.push_back(argv[i]);
    }

    //User input parsing
    //TODO: Improve this a lot

    CLI cli = CLI();

    path_vector = cli.getPath(argv_vector);

    if (cli.checkForErrors()) 
    {
        return cli.getError();
    }

    //TODO: CFG file

    //Main algorithm
    //TODO: Optimize it

    Render render = Render();

    if (render.init(path_vector)) 
    {
        render.loop();
        render.closeSDL();
    }
    else
    {
        render.closeSDL();
        return (-1);
    }

    return 0;
}
