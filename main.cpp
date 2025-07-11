#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>
#include "ArrayVisualizer.h"
#include "LinkedListVisualizer.h"
#include "MenuUI.h"

enum class AppState {
	MENU,
	ARRAY_VISUALIZER,
	LINKED_LIST_VISUALIZER,
	STACK_VISUALIZER,
	QUEUE_VISUALIZER,
	GRAPH_VISUALIZER,
	TREES_VISUALIZER
};

int main() {
	sf::RenderWindow window(sf::VideoMode({ 800,600 }), "Visualizer App");
	window.setFramerateLimit(60);
	
	auto image = sf::Image{};
	if (!image.loadFromFile("Icon/icon.png")) {
		std::cerr << "ERROR COULD NOT LOAD ICON IMAGE" << std::endl;
		return -1;
	}
	window.setIcon(image.getSize(), image.getPixelsPtr());
	tgui::Gui gui{window};

	AppState appState =  AppState::MENU;

	sf::Font font;
	if (!font.openFromFile("Fonts/ArialCE.ttf")) {
		std::cerr << "ERROR:: COULD NOT OPEN FROM FILE" << std::endl;
		return -1;
	}

	MenuUI menu(gui);
	ArrayVisualizer arrayVisualizer(window, menu);
	LinkedListVisualizer linkedList(window, menu, font);

	while (window.isOpen()) {

		while (const std::optional event = window.pollEvent()) {
			gui.handleEvent(*event);
			if (event->is<sf::Event::Closed>())
				window.close();


			else if (appState == AppState::ARRAY_VISUALIZER) {
				arrayVisualizer.handleEvent(*event);
			}
			else if (appState == AppState::LINKED_LIST_VISUALIZER) {
				linkedList.handleEvent(*event);
			}
		}
		window.clear(sf::Color(sf::Color::Green));


		if (appState == AppState::MENU) {
			gui.draw();
			if (menu.selectedVisualizer == MenuUI::VisualizerType::ARRAY) {
				menu.selectedVisualizer = MenuUI::VisualizerType::NONE;
				gui.removeAllWidgets();
				appState = AppState::ARRAY_VISUALIZER;
			}
			else if (menu.selectedVisualizer == MenuUI::VisualizerType::LINKED_LIST) {
				menu.selectedVisualizer = MenuUI::VisualizerType::NONE;
				gui.removeAllWidgets();
				appState = AppState::LINKED_LIST_VISUALIZER;
			}
		}
		else if (appState == AppState::ARRAY_VISUALIZER) {
			arrayVisualizer.update(1.0f / 60.f);
			arrayVisualizer.render();
					
			if(arrayVisualizer.backToMenuRequested) {
				arrayVisualizer.reset();
				menu.buildMenu(gui);
				menu.hidePanels();
				appState = AppState::MENU;
			}
		}
		else if (appState == AppState::LINKED_LIST_VISUALIZER) {
			linkedList.update(1.0f / 60.0f);
			linkedList.render();

			if (linkedList.backToMenuRequested) {
				linkedList.reset();
				menu.buildMenu(gui);
				menu.hidePanels();
				appState = AppState::MENU;
			}
		}

		window.display();
	}

	return 0;
}