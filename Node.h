#pragma once

#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>

struct VisualNode {
	int value;
	VisualNode* next;

	sf::RectangleShape box;
	sf::Text valueText;
	sf::VertexArray arrow;
	sf::VertexArray arrowHead;

	VisualNode(int val, sf::Font &font) : 
		value(val), next(nullptr), arrow(sf::PrimitiveType::Lines,2), valueText(font), arrowHead(sf::PrimitiveType::Triangles, 3){
		box.setOrigin(box.getGeometricCenter());
		box.setSize({ 60.f,60.f });
		box.setFillColor(sf::Color::White);
		box.setOutlineColor(sf::Color::Black);
		box.setOutlineThickness(2.0f);

		valueText.setString(std::to_string(val));
		valueText.setCharacterSize(24);
		valueText.setFillColor(sf::Color::Black);

		arrow[0].color = sf::Color::Black;
		arrow[1].color = sf::Color::Black;

		arrowHead[0].color = sf::Color::Red;
		arrowHead[1].color = sf::Color::Red;
		arrowHead[2].color = sf::Color::Red;
	}

	
};


#endif // !NODE_H
