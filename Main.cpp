//#include "Ghost.h"
//#include "PacMan.h"
#include "Window.h"
#include <iostream>
#include "Map_Editor.h"

// VYSVETLIT PROC JE TO ANIMOVANE JAKO OD RETARDA
// Opravit nebo smazat Map editor

int main()
{
	Map_Editor editor(window, 10, 10);
	for(;;)
	{
		editor.run();
	}
	return 0;
}


//int main()
//{
//    window.setFramerateLimit(Options::fps);
//    auto time = std::chrono::high_resolution_clock::now();
//    Ghost ghost(sf::Color::Yellow, time);
//    PacMan pacman(sf::Color::Blue, time);
//    ghost.pos = { 700, 400 };
//    pacman.pos = { 500, 400 };
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event)) 
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            
//        }
//
//        window.clear();
//
//        pacman.draw();
//        ghost.draw();
//        //window.draw(x_axis, 2, sf::Lines);
//        //window.draw(y_axis, 2, sf::Lines);
//
//        window.display();
//    }
//    return 0;
//}
