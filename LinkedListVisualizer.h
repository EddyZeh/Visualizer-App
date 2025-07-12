#pragma once

#ifndef _LINKEDLISTVISUALIZER_H
#define _LINKEDLISTVISUALIZER_H

#include <vector>
#include "Visualizer.h"
#include "MenuUI.h"
#include "Node.h"

class LinkedListVisualizer : public Visualizer {
	
public:

	bool backToMenuRequested = false;

	LinkedListVisualizer(sf::RenderWindow &win, MenuUI &UI, sf::Font& font);
	~LinkedListVisualizer();

	void appendNode(int value);
	void deleteNode(int value);
	void searchNode(int value);

	virtual void reset() override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
	virtual void handleEvent(const sf::Event& event) override;

private:
	sf::RenderWindow& window;
	MenuUI* ui;
	sf::Font& _font;
	//std::vector<VisualNode*> nodes;
	VisualNode* head;

	void updateLayout();
};

#endif // !_LINKEDLISTVISUALIZER_H
