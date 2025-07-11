#pragma once

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <SFML/Graphics.hpp>

class Visualizer {
public:
	virtual void handleEvent(const sf::Event&) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
	virtual void reset() = 0;
	virtual ~Visualizer() = default;

};

#endif // !VISUALIZER_H
