#include "LinkedListVisualizer.h"

void LinkedListVisualizer::updateLayout() {

}

LinkedListVisualizer::LinkedListVisualizer(sf::RenderWindow &win, MenuUI& UI, sf::Font& font) : window(win), _font(font){
	ui = &UI;
}

LinkedListVisualizer::~LinkedListVisualizer() {
	reset();
}

void LinkedListVisualizer::appendNode(int value) {
	auto* newNode = new VisualNode(value, _font);
	nodes.push_back(newNode);
	updateLayout();
}

void LinkedListVisualizer::deleteNode(int value) {

}

void LinkedListVisualizer::searchNode(int value) {

}

void LinkedListVisualizer::reset() {

	backToMenuRequested = false;

	if (nodes.empty()) return;

	for (auto* node : nodes) {
		delete node;
	}
	nodes.clear();
}
void LinkedListVisualizer::update(float deltaTime) {
	const float startX = 100.0f;
	const float startY = 200.0f;
	const float spacing = 100.0f;

	for (size_t i = 0; i < nodes.size(); ++i) {
		VisualNode* node = nodes[i];
		float x = startX + i * spacing;

		sf::FloatRect textBounds = node->valueText.getLocalBounds();

		node->box.setPosition({ x, startY });
		node->valueText.setPosition(
			{ x + node->box.getSize().x / 2.f - node->valueText.getLocalBounds().size.x / 2.f,
			startY + node->box.getSize().y / 2.0f - node->valueText.getCharacterSize() / 1.5f
			});

		if (i < nodes.size() - 1) {
			node->arrow[0].position = sf::Vector2f({ x + node->box.getSize().x, startY + node->box.getSize().y / 2.0f });
			node->arrow[1].position = sf::Vector2f({ x + spacing, startY + node->box.getSize().y / 2.0f });

			node->arrowHead[0].position = node->arrow[1].position;
			node->arrowHead[1].position = node->arrow[1].position - sf::Vector2f({ 10,  10 });
			node->arrowHead[2].position = node->arrow[1].position - sf::Vector2f({ 10, -10 });
		}
	}
}
void LinkedListVisualizer::render() {
	for (auto* node : nodes) {
		window.draw(node->box);
		window.draw(node->valueText);
		if (node != nodes.back()) {
			window.draw(node->arrow);
			window.draw(node->arrowHead);
		}
	}
}
void LinkedListVisualizer::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			backToMenuRequested = true;
		else if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
			appendNode(5);
		}
		else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
			reset();
		}
	}
}