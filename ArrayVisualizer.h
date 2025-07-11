#pragma once

#ifndef ARRAYVISUALIZER_H
#define ARRAYVISUALIZER_H

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <random>
#include <algorithm>
#include <stack>
#include "Visualizer.h"
#include "MenuUI.h"



class ArrayVisualizer : public Visualizer {

private:

	enum class SortAlgorithm { NONE, BUBBLE, SELECTION, INSERTION, MERGE, QUICK, SHELL_SORT };

	enum class SwapPhase { MOVING, FLASHING, DONE };


	sf::RenderWindow &window;
	tgui::Gui gui;
	std::vector<int> array;
	std::vector<sf::RectangleShape> bars;
	bool isSorting = false, isAnimating = false, sorted = false;
	int outer = 0, inner = 0, minIdx = 0;
	float animationSpeed;
	MenuUI *ui;

	struct SwapTask {
		int index1, index2;
		float progress = 0.0f, flashTimer = 0.0f;
		sf::Vector2f targetPos1, targetPos2, startPos1, startPos2;
		SwapPhase phase = SwapPhase::DONE;
		bool inProgress = false;
	};

	SwapTask currentSwap;

	struct MergeTask {
		int left, right;
		bool isMergePhase;
	};

	std::stack<MergeTask> mergeTasks;
	std::vector<int> tempArray;

	int activeLeft = -1;
	int activeRight = -1;
	float mergeAnimationTimer = 0.f;
	bool waitingForMergeAnimation = false;

	struct QuickTask {
		int left;
		int right;
	};


	std::stack<QuickTask> quickTasks;
	int pivotIndex = -1;
	int q_left, q_right;
	int pivotVal;
	int i, j;
	bool partitionInProgress = false;

	int gap;
	int p, q;
	bool qChanged = false;

	void buildUI();
	void recomputeBarLayout();

	void bubbleSortStep();
	void selectionSortStep();
	void insertionSortStep();
	void mergeSortStep();
	void quickSortStep();
	void shellSortStep();
	void prepareSwap(int idx1, int idx2);
	void handleAnimation(float deltaTime);
	void highlightSubArray(int left, int right, sf::Color color);
	void highlightPartionPointer();
	void resetMergeSortState();
	void resetQuickSortState();

public:
	explicit ArrayVisualizer(sf::RenderWindow& win, MenuUI &UI) : window(win),gui(win) { 
		reset(); 
		ui = &UI;
	}

	virtual void handleEvent(const sf::Event& event) override;
	virtual void update(float deltaTime) override;
	virtual void render() override;
	virtual void reset() override;
	bool backToMenuRequested = false;
};

#endif // !ARRAYVISUALIZER

