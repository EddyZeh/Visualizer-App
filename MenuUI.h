#pragma once

#ifndef MENUUIH_H
#define MENUUIH_H

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

enum class SortingAlgorithm {
	NONE,
	BUBBLE,
	SELECTION,
	INSERTION,
	MERGE,
	QUICK,
	SHELL_SORT
};

class MenuUI {

public:

	enum class VisualizerType { NONE, ARRAY, LINKED_LIST, STACK, QUEUE, GRAPH, TREES };
	
	enum class LinkedListType { NONE, SINGLE, DOUBLE, CIRCULAR, DOUBLE_CIRCULAR };

	tgui::Panel::Ptr structurePanel;
	tgui::Panel::Ptr algorithmPanel;
	tgui::Panel::Ptr linkedListPanel;

	VisualizerType selectedVisualizer = VisualizerType::NONE;
	SortingAlgorithm selectedAlgo = SortingAlgorithm::NONE;
	LinkedListType selectedLinkedList;
	MenuUI(tgui::BackendGui& gui);
	
	void addAlgoButton(const std::string& text, float yPercent, SortingAlgorithm algo);
	void addLinkedListButton(const std::string& text, float yPercent, LinkedListType LLType);
	void buildMenu(tgui::BackendGui& gui);

	void showAlgorithmPanel();
	void hidePanels();
	SortingAlgorithm getSelectedAlgorithm();
};

#endif 
