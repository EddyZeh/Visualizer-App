#include "ArrayVisualizer.h"
#include <cassert>

void ArrayVisualizer::buildUI(){
	gui.removeAllWidgets();


	tgui::Panel::Ptr panel = tgui::Panel::create({ "20%", "100%" });
	panel->getRenderer()->setBackgroundColor(sf::Color(50, 50, 50));
	panel->setVisible(false);

	auto shuffleBtn = tgui::Button::create("Shuffle");
	shuffleBtn->setSize({ "90%", 40 });
	shuffleBtn->setPosition({ "5%", "5%" });
	shuffleBtn->onClick([this] {
		std::shuffle(array.begin(), array.end(), std::mt19937{ std::random_device{}() });
		isSorting = false;
		isAnimating = false;
		outer = 0;
		inner = 0;
		recomputeBarLayout();
		});
	panel->add(shuffleBtn);

	auto revealPanel = tgui::Button::create("Reveal Panel");
	revealPanel->setSize({ "15%", 40 });
	revealPanel->setPosition({ "2%", "3%" });
	revealPanel->onClick([panel, revealPanel] {
		panel->setVisible(true);
		revealPanel->setVisible(false);
		});

	auto sortButton = tgui::Button::create("Sort");
	sortButton->setSize({ "90%", 40 });
	sortButton->setPosition({ "5%","15%" });
	sortButton->onClick([this, panel, revealPanel] {
		if (!isSorting && !isAnimating) {
			panel->setVisible(false);
			revealPanel->setVisible(true);
			isSorting = true;
			if (ui->getSelectedAlgorithm() == SortingAlgorithm::INSERTION)
				outer = 1;
			else if (ui->getSelectedAlgorithm() == SortingAlgorithm::MERGE)
				resetMergeSortState();
			else if (ui->getSelectedAlgorithm() == SortingAlgorithm::QUICK)
				resetQuickSortState();
				outer = 0;

			inner = 0;
		}
		});
	panel->add(sortButton);

	auto backButton = tgui::Button::create("Back");
	backButton->setSize({ "90%", 40 });
	backButton->setPosition({ "5%", "25%" });
	backButton->onClick([this] {
		backToMenuRequested = true;
		});

	panel->add(backButton);	


	auto hidePanel = tgui::Button::create("Hide Panel");
	hidePanel->setSize({ "90%", 40 });
	hidePanel->setPosition({ "5%", "90%" });
	hidePanel->onClick([panel, revealPanel] {
		panel->setVisible(false);
		revealPanel->setVisible(true);
		});
	panel->add(hidePanel);


	gui.add(panel);
	gui.add(revealPanel);
}

void ArrayVisualizer::recomputeBarLayout(){

	const int numBars = static_cast<int>(array.size());

	const int maxValue = 100;

	float barWidth = static_cast<float>(window.getSize().x / numBars);

	for (int i = 0; i < numBars; i++) {
		float heightRatio = static_cast<float>(array[i]) / (maxValue);
		float barHeight = heightRatio * static_cast<float>(window.getSize().y * 0.8f);

		bars[i].setSize(sf::Vector2f({ barWidth - 2, -barHeight }));
		bars[i].setPosition({ i * barWidth, static_cast<float>(window.getSize().y) - 50 });
		bars[i].setFillColor(sf::Color::White);
	}
}

void ArrayVisualizer::bubbleSortStep(){
	if (isSorting && !isAnimating) {
		if (outer < bars.size() - 1) {
			if (inner < bars.size() - outer - 1) {
				if (array[inner] > array[inner + 1]) {

					prepareSwap(inner, inner + 1);
				}
				inner++;
			}
			else {
				inner = 0;
				outer++;
			}
		}
		else {
			sorted = true;
			isSorting = false;
		}
	}


	for (size_t i = 0; i < bars.size(); ++i) {
		if (isSorting && i >= bars.size() - outer) {
			bars[i].setFillColor(sf::Color::Green);
		}
		else {
			if (sorted) {
				bars[i].setFillColor(sf::Color::Green);
			}
			else
				bars[i].setFillColor(sf::Color::White);
		}
	}

	if (isSorting && outer < bars.size() - 1 && inner < bars.size() - outer - 1) {
		bars[inner].setFillColor(sf::Color::Red);
		bars[inner + 1].setFillColor(sf::Color::Red);
	}
}

void ArrayVisualizer::selectionSortStep(){
	if (isAnimating) return;
	if (isSorting && !isAnimating) {
		if (outer < array.size() - 1) {
			minIdx = outer;

			for (int i = outer + 1; i < array.size(); ++i) {
				if (array[i] < array[minIdx]) {
					minIdx = i;
				}
			}
			if (minIdx != outer) {
				prepareSwap(minIdx, outer);
			}
			outer++;
		}
		else {
			isSorting = false;
			sorted = true;
		}
	}
}

void ArrayVisualizer::insertionSortStep(){

	if (isSorting && !isAnimating) {
		if (outer < array.size()) {
			if (inner > 0 && array[inner - 1] > array[inner]) {
				prepareSwap(inner, inner - 1);
			}
			else {
				outer++;
				inner = outer;
			}
		}
		else {
			isSorting = false;
			sorted = true;
		}
	}
}

void ArrayVisualizer::mergeSortStep(){
	
	if (mergeTasks.empty()) {
		isSorting = false;
		sorted = true;
		return;
	}

	if (isAnimating || waitingForMergeAnimation) return;

	if (isSorting && !isAnimating) {
		MergeTask task = mergeTasks.top();
		mergeTasks.pop();

		int left = task.left;
		int right = task.right;


		activeLeft = left;
		activeRight = right;

		if (!task.isMergePhase) {


			if (left >= right) {
				return;
			}

			int mid = left + (right - left) / 2;

			highlightSubArray(left, right, sf::Color(255, 215, 0));
			highlightSubArray(mid + 1, right, sf::Color(135, 206, 250));

			mergeTasks.push({ left, right, true });

			mergeTasks.push({ mid + 1, right, false });

			mergeTasks.push({ left, mid, false });

		}
		else {
			int mid = left + (right - left) / 2;
			int i = left;
			int j = mid + 1;
			std::vector<int> temp;

			while (i <= mid && j <= right) {
				if (array[i] <= array[j]) {
					temp.push_back(array[i]);
					i++;
				}
				else {
					temp.push_back(array[j]);
					j++;
				}
			}

			while (i <= mid) temp.push_back(array[i++]);
			while (j <= right) temp.push_back(array[j++]);

			for (int k = 0; k < temp.size(); ++k) {
				array[left + k] = temp[k];

				float heightRatio = static_cast<float>(temp[k]) / 100.0f;
				float barHeight = heightRatio * static_cast<float>(window.getSize().y * 0.8f);

				bars[left + k].setSize({ bars[left + k].getSize().x, -barHeight });
				bars[left + k].setFillColor(sf::Color::Green);
			}

			mergeAnimationTimer = 0.2f;
			waitingForMergeAnimation = true;
		}
	}
}

void ArrayVisualizer::quickSortStep(){
	
	if (!isSorting || isAnimating) return;

	if (partitionInProgress) {
		if (j < q_right) {
			if (array[j] < pivotVal) {
				i++;
				if (i != j) {
					prepareSwap(i, j);
				}
			}
			j++;
			highlightPartionPointer();
			return;
		}
		else {
			i++;
			if (i != q_right) {
				prepareSwap(i, q_right);
			}

			partitionInProgress = false;
			if (q_left < i - 1)
				quickTasks.push({ q_left, i - 1 });
			if (i + 1 < q_right)
				quickTasks.push({ i + 1, q_right });

			return;
		}
	}

	if (quickTasks.empty()) {
		isSorting = false;
		sorted = true;
		return;
	}

	QuickTask task = quickTasks.top();
	quickTasks.pop();

	q_left = task.left;
	q_right = task.right;
	pivotVal = array[q_right];
	pivotIndex = q_right;
	i = q_left - 1;
	j = q_left;
	partitionInProgress = true;

	highlightPartionPointer();
	
}

void ArrayVisualizer::shellSortStep(){
	if (isSorting && !isAnimating) {
		if (gap > 0) {
			if (p < bars.size()) {
				if (!qChanged)
					q = p;

				if (q >= gap && array[q - gap] > array[q]) {
					prepareSwap(q - gap, q);
					qChanged = true;
				}
				else {
					p++;
					qChanged = false;
				}

			}
			else {
				gap /= 2;
				p = gap;
			}
		}
		else {
			isSorting = false;
			sorted = true;
			std::cout << "Sorting complete!" << std::endl;
		}
	}
}

void ArrayVisualizer::prepareSwap(int idx1, int idx2){
	currentSwap.index1 = idx1;
	currentSwap.index2 = idx2;
	currentSwap.startPos1 = bars[idx1].getPosition();
	currentSwap.startPos2 = bars[idx2].getPosition();
	currentSwap.targetPos1 = bars[idx2].getPosition();
	currentSwap.targetPos2 = bars[idx1].getPosition();

	currentSwap.progress = 0.0f;
	currentSwap.flashTimer = 0.0f;
	currentSwap.phase = SwapPhase::MOVING;
	currentSwap.inProgress = true;
	isAnimating = true;

	bars[idx1].setFillColor(sf::Color::Yellow);
	bars[idx2].setFillColor(sf::Color::Yellow);
}

void ArrayVisualizer::handleAnimation(float deltaTime){
	if (isAnimating && currentSwap.inProgress) {
		auto& bar1 = bars[currentSwap.index1];
		auto& bar2 = bars[currentSwap.index2];
		
		if(ui->getSelectedAlgorithm() == SortingAlgorithm::BUBBLE) {
			sf::Vector2f pos1 = bar1.getPosition();
			sf::Vector2f pos2 = bar2.getPosition();


			sf::Vector2f dir1 = currentSwap.targetPos1 - pos1;
			sf::Vector2f dir2 = currentSwap.targetPos2 - pos2;

			float dist1 = std::sqrt(static_cast<float>(std::pow(dir1.x, 2) + std::pow(dir1.y, 2)));
			float dist2 = std::sqrt(static_cast<float>(std::pow(dir2.x, 2) + std::pow(dir2.y, 2)));


			float step = animationSpeed * deltaTime;


			if (dist1 <= step && dist2 <= step) {
				bar1.setPosition(currentSwap.targetPos1);
				bar2.setPosition(currentSwap.targetPos2);
								
				std::swap(array[currentSwap.index1], array[currentSwap.index2]);
				std::swap(bar1, bar2);
				currentSwap.inProgress = false;
				isAnimating = false;
				isSorting = true;
				inner--;
			}
			else {
				bar1.move({ step * (dir1.x / dist1), step * (dir1.y / dist1) });
				bar2.move({ step * (dir2.x / dist2), step * (dir2.y / dist2) });
			}
		}
		else {
			if (currentSwap.phase == SwapPhase::MOVING) {
				float duration = 0.3f;

				currentSwap.progress += deltaTime / duration;
				if (currentSwap.progress >= 1.0f) {
					currentSwap.progress = 1.0f;
				}



				auto lerp = [](sf::Vector2f a, sf::Vector2f b, float t) {
					return a + (b - a) * t;
					};

				bar1.setPosition(lerp(currentSwap.startPos1, currentSwap.targetPos1, currentSwap.progress));
				bar2.setPosition(lerp(currentSwap.startPos2, currentSwap.targetPos2, currentSwap.progress));

				if (currentSwap.progress >= 1.0f) {
					bar1.setPosition(currentSwap.targetPos1);
					bar2.setPosition(currentSwap.targetPos2);

					std::swap(array[currentSwap.index1], array[currentSwap.index2]);
					std::swap(bars[currentSwap.index1], bars[currentSwap.index2]);

					bars[currentSwap.index1].setFillColor(sf::Color::Green);
					bars[currentSwap.index2].setFillColor(sf::Color::Green);
					currentSwap.flashTimer = 0.0f;
					currentSwap.phase = SwapPhase::FLASHING;

				}
			}
			else if (currentSwap.phase == SwapPhase::FLASHING) {
				float flashDuration = 0.2f;
				currentSwap.flashTimer += deltaTime;

				if (currentSwap.flashTimer >= flashDuration) {
					bars[currentSwap.index1].setFillColor(sf::Color::White);
					bars[currentSwap.index2].setFillColor(sf::Color::White);

					currentSwap.inProgress = false;
					isAnimating = false;

					if (ui->getSelectedAlgorithm() == SortingAlgorithm::INSERTION) {
						inner--;
					}

					if (ui->getSelectedAlgorithm() == SortingAlgorithm::SHELL_SORT) {
						if (qChanged) {
							q -= gap;
						}
					}

					currentSwap.phase = SwapPhase::DONE;
				}
			}
		}

	}
}

void ArrayVisualizer::highlightSubArray(int left, int right, sf::Color color) {
	for (int i = left; i <= right; ++i) {
		bars[i].setFillColor(color);
	}
}

void ArrayVisualizer::resetMergeSortState() {
	while (!mergeTasks.empty()) mergeTasks.pop();
	mergeTasks.push({ 0, static_cast<int>(array.size()) - 1, false });
	activeLeft = -1;
	activeRight = -1;
	mergeAnimationTimer = 0.0f;
	waitingForMergeAnimation = false;
}

void ArrayVisualizer::resetQuickSortState() {
	while (!quickTasks.empty()) quickTasks.pop();
	quickTasks.push({ 0, static_cast<int>(array.size()) - 1 });
	pivotIndex = -1;
	partitionInProgress = false;
}

void ArrayVisualizer::highlightPartionPointer() {
	for (auto& bar : bars) {
		bar.setFillColor(sf::Color::White);
	}

	highlightSubArray(q_left, q_right, sf::Color(255, 215,0));

	bars[pivotIndex].setFillColor(sf::Color::Blue);

	if (i >= q_left && i <= q_right) {
		bars[i].setFillColor(sf::Color::Magenta);
	}
	if (j >= q_left && j <= q_right) {
		bars[j].setFillColor(sf::Color::Red);
	}
}

void ArrayVisualizer::handleEvent(const sf::Event& event){
	gui.handleEvent(event);
	if (const auto *KeyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (event.is<sf::Event::Resized>()) {
			recomputeBarLayout();
		}
		if (KeyPressed->scancode == sf::Keyboard::Scancode::Escape) {
			backToMenuRequested = true;
		}
		else if (KeyPressed->scancode == sf::Keyboard::Scancode::Space) {
			isSorting = true;

			if (ui->getSelectedAlgorithm() == SortingAlgorithm::INSERTION)
				outer = inner = 1;
			else if (ui->getSelectedAlgorithm() == SortingAlgorithm::MERGE) {
				activeLeft = -1;
				activeRight = -1;
				mergeAnimationTimer = 0.0f;
				waitingForMergeAnimation = false;
			}
			else if (ui->getSelectedAlgorithm() == SortingAlgorithm::QUICK) {
				pivotIndex = -1;
				partitionInProgress = false;
			}
			else if (ui->getSelectedAlgorithm() == SortingAlgorithm::SHELL_SORT) {
				gap = bars.size() / 2;
				p = gap;
			}
			else
				outer = inner = 0;
		}
	}
}



void ArrayVisualizer::update(float deltaTime){

	switch (ui->getSelectedAlgorithm()) {
	case SortingAlgorithm::BUBBLE:
		bubbleSortStep();
		break;

	case SortingAlgorithm::SELECTION:
		selectionSortStep();
		break;

	case SortingAlgorithm::INSERTION:
		insertionSortStep();
		break;

	case SortingAlgorithm::MERGE:
		mergeSortStep();
		if (mergeAnimationTimer > 0.f) {
			mergeAnimationTimer -= deltaTime;
			if (mergeAnimationTimer <= 0.f) {
				for (int i = activeLeft; i <= activeRight; ++i) {
					bars[i].setFillColor(sf::Color::White);
				}

				waitingForMergeAnimation = false;
			}
		}
		break;

	case SortingAlgorithm::QUICK:
		quickSortStep();
		break;

	case SortingAlgorithm::SHELL_SORT:
		shellSortStep();
		break;
	}


	handleAnimation(deltaTime);

}

void ArrayVisualizer::render(){
	window.clear(sf::Color(20, 20, 30));

	for (const auto& bar : bars) {
		window.draw(bar);
	}
	gui.draw();
}

void ArrayVisualizer::reset(){
	backToMenuRequested = false;

	isSorting = false;
	sorted = false;
	isAnimating = false;
	currentSwap.inProgress = false;
	animationSpeed = 200.0f;
	outer = 0;
	inner = 0;

	bars.clear();
	array.clear();

	const int numBars = 50;
	const int maxValue = 100;

	gap = numBars / 2;
	p = gap;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(10, maxValue);


	for (int i = 0; i < numBars; ++i) {
		array.push_back(dist(gen));

		sf::RectangleShape bar;
		bars.push_back(bar);
	}
	
	tempArray.resize(array.size());
	mergeTasks = {};

	mergeTasks.push({ 0, static_cast<int>(array.size()) - 1, false });
	
	activeLeft = -1;
	activeRight = -1;

	quickTasks = {};
	quickTasks.push({ 0, static_cast<int>(array.size()) - 1 });

	recomputeBarLayout();

	buildUI();

}


