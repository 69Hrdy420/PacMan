#include "cmd.h"

void cmd::run()
{
	for (;;)
	{
		std::cout << "press 'h' for help" << std::endl;
		char c = std::cin.get();
		switch (c)
		{
		case 'h':
		{
			std::cout << "---------------" << std::endl;
			std::cout << "Basic Commands:" << std::endl << std::endl;
			std::cout << "m - map editor" << std::endl;
			std::cout << "p - play with default map" << std::endl;
			std::cout << "e - exit app" << std::endl;
			std::cout << "h - Turns on GUIDE MODE" << std::endl;
			std::cout << "-----------------------" << std::endl;
			std::cout << "GUIDE MODE Commands:" << std::endl << std::endl;
			std::cout << "m - map editor guide" << std::endl;
			std::cout << "g - game guide" << std::endl;
			std::cout << "e - exit GUIDE MODE" << std::endl;
			std::cout << "-------------------" << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			bool exit = false;
			while (!exit)
			{
				std::cout << "GUIDE MODE" << std::endl;
				std::cout << "----------" << std::endl;
				char d = std::cin.get();
				switch (d)
				{
				case 'e':
					exit = true;
					break;
				case 'm':
					std::cout << "Map Editor Guide" << std::endl << std::endl;
					std::cout << "In map editor, you see peers (white points). Each peer can either have Coin, Power-up, Ghost spawn, PacMan spawn or nothing (white point). You can connect peers with your mouse, if you want to allow entities to move from one peer to another. The walls around paths will generate automatically after you enter Map Loader the Game" << std::endl;
					std::cout << "Keep in mind, that walls will appear around your path in distance of one peer, so if you want to create two separate paths, they should be at least one peer between them" << std::endl;
					std::cout << "Drag mouse left button to create paths between peers" << std::endl;
					std::cout << "Drag mouse right button to remove paths between peers" << std::endl;
					std::cout << "Use mouse wheel to zoom in or out" << std::endl;
					std::cout << "Use C to place coins" << std::endl;
					std::cout << "Use P to place PacMan spawn point" << std::endl;
					std::cout << "Use G to place ghost spawn point" << std::endl;
					std::cout << "Use B to place Boost for PacMan" << std::endl;
					std::cout << "Use D to delete any object on peer" << std::endl;
					std::cout << "Press arrow keys to resize map" << std::endl;
					std::cout << "Press L if you want to look how walls will look around your paths" << std::endl;
					std::cout << "Press G if you want to enter the game. Keep in mind that i was too lazy to make the game for more then two players, so you will always be able to control only two entities. Also you should check how the game will be generated with Map Loader by pressing L" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << "e - exit GUIDE MODE" << std::endl;
					std::cout << "-------------------" << std::endl;
					break;
				case 'g':
					std::cout << "Game Guide" << std::endl << std::endl;
					std::cout << "Entity 1 controls - arrow keys, and right shift to stop moving" << std::endl;
					std::cout << "Entity 2 controls - asdw, and space to stop moving" << std::endl << std::endl;
					std::cout << "This project is unfinished, especially the game part has some things missing:" << std::endl;
					std::cout << "Game doesent support teleports from one side of the map to another yet, even tho it is possible to create them in map editor." << std::endl;
					std::cout << "Ghost AI is missing, because it wasnt my goal of this project to create it." << std::endl;
					std::cout << "PacMan keeps track of how many coins he ate, he just doesent tell you" << std::endl;
					std::cout << "If entity dies, it respawns at its original position" << std::endl;
					std::cout << "Game isnt possible to resize due to fact that i fucked up resize function for ghosts" << std::endl;
					std::cout << "--------------------------------------------------" << std::endl;
					std::cout << "e - exit GUIDE MODE" << std::endl;
					std::cout << "-------------------" << std::endl;
					break;
				default:
					break;
				}
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			break;
		}
		case 'e':
		{
			return;
		}
		case 'm':
		{
			Window window("Map Editor");
			Map_Editor editor(window, 10, 10);
			for (;;)
			{
				window.window.clear();
				if (!editor.run())
					break;
				window.window.display();
			}
			break;
		}
		case 'p':
		{
			std::vector<Line> walls;
			std::vector<std::vector<Peer>> peers;
			Window window("Pac-Man");
			window.window.setFramerateLimit(Options::fps);
			{
				Map_Editor editor(window, 10, 10);
				editor.load(Options::get_default_map());
				Map_Loader loader(window, editor.get_paths(), editor.get_objects(), {}, {});
				walls = loader.get_walls();
				peers = loader.get_peers();
			}
			std::vector<Entity*> entities;
			PacMan pacman(sf::Color::Yellow, { 12, 22 }, std::chrono::high_resolution_clock::now());
			Ghost ghost(sf::Color::Red, { 13, 10 }, std::chrono::high_resolution_clock::now());
			entities.push_back(&pacman);
			entities.push_back(&ghost);
			Game game(window.window, walls, entities, peers);
			Input input;
			input.add(pacman, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::Space);
			input.add(ghost, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::RShift);
			for (;;)
			{
				window.window.clear();
				sf::Event event;
				if (window.window.pollEvent(event))
				{
					window.run(event);
					if (event.type == sf::Event::KeyPressed)
					{
						input.run(event.key.code);
					}
				}
				game.run();
				pacman.eat(game.eat(pacman.peer));
				window.window.display();
				if (!window.window.isOpen())
					break;
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}
}
