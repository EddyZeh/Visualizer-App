#include "MenuUI.h"

MenuUI::MenuUI(tgui::BackendGui& gui){
	this->buildMenu(gui);
}

void MenuUI::addAlgoButton(const std::string& text, float yPercent, SortingAlgorithm algo){
	auto btn = tgui::Button::create(text);
	btn->setSize({ "90%", 40 });
	btn->setPosition({ "5%", tgui::String(yPercent) + "%" });
	btn->onClick([this, algo] {
		this->selectedVisualizer = VisualizerType::ARRAY;
		selectedAlgo = algo;
		});

	algorithmPanel->add(btn);
}

void MenuUI::addLinkedListButton(const std::string& text, float yPercent, LinkedListType LLType){
	auto btn = tgui::Button::create(text);
	btn->setSize({ "90%",40 });
	btn->setPosition({ "5%", tgui::String(yPercent) + "%" });
	btn->onClick([this, LLType] {
		this->selectedVisualizer = VisualizerType::LINKED_LIST;
		selectedLinkedList = LLType;
		});

	linkedListPanel->add(btn);
}

void MenuUI::buildMenu(tgui::BackendGui& gui){
	structurePanel = tgui::Panel::create({ "25%","100%" });
	structurePanel->getRenderer()->setBackgroundColor(sf::Color(50, 50, 50));

	auto label = tgui::Label::create("Choose Data Structure");
	label->setTextSize(20);
	label->setPosition({ "5%", "5%" });
	structurePanel->add(label);

	auto arrayBtn = tgui::Button::create("Array Button");
	arrayBtn->setSize({"90%", 40});
	arrayBtn->setPosition({ "5%","10%" });
	arrayBtn->onClick([this]() {
		this->linkedListPanel->setVisible(false);
		this->algorithmPanel->setVisible(true);
		});
	structurePanel->add(arrayBtn);

	auto LinkedListBtn = tgui::Button::create("Linked List Button");
	LinkedListBtn->setSize({ "90%", 40 });
	LinkedListBtn->setPosition({ "5%","15%" });
	LinkedListBtn->onClick([this]() { 
		this->linkedListPanel->setVisible(true); });
	structurePanel->add(LinkedListBtn);

	auto StackBtn = tgui::Button::create("Stack Button");
	StackBtn->setSize({ "90%", 40 });
	StackBtn->setPosition({ "5%","20%" });
	StackBtn->onClick([this]() { this->selectedVisualizer = VisualizerType::STACK; });
	structurePanel->add(StackBtn);

	auto QueueBtn = tgui::Button::create("Queue Button");
	QueueBtn->setSize({ "90%", 40 });
	QueueBtn->setPosition({ "5%","25%" });
	QueueBtn->onClick([this]() { this->selectedVisualizer = VisualizerType::QUEUE; });
	structurePanel->add(QueueBtn);

	auto GraphBtn = tgui::Button::create("Graph Button");
	GraphBtn->setSize({ "90%", 40 });
	GraphBtn->setPosition({ "5%","30%" });
	GraphBtn->onClick([this]() { this->selectedVisualizer = VisualizerType::GRAPH; });
	structurePanel->add(GraphBtn);

	auto TreesBtn = tgui::Button::create("Trees Button");
	TreesBtn->setSize({ "90%", 40 });
	TreesBtn->setPosition({ "5%","35%" });
	TreesBtn->onClick([this]() { this->selectedVisualizer = VisualizerType::TREES; });
	structurePanel->add(TreesBtn);
	
	gui.add(structurePanel);

	algorithmPanel = tgui::Panel::create({ "25%", "100%" });
	algorithmPanel->setPosition({ "25%","0%" });
	algorithmPanel->getRenderer()->setBackgroundColor(sf::Color(70, 70, 70));
	algorithmPanel->setVisible(false);

	auto algoLabel = tgui::Label::create("Choose Sorting Algorithm");
	algoLabel->setTextSize(20);
	algoLabel->setPosition({ "5%","5%" });
	algorithmPanel->add(algoLabel);

	addAlgoButton("Bubble Sort", 15, SortingAlgorithm::BUBBLE);
	addAlgoButton("Selection Sort", 25, SortingAlgorithm::SELECTION);
	addAlgoButton("Insertion Sort", 35, SortingAlgorithm::INSERTION);
	addAlgoButton("Merge Sort", 45, SortingAlgorithm::MERGE);
	addAlgoButton("Quick Sort", 55, SortingAlgorithm::QUICK);
	addAlgoButton("Shell Sort", 65, SortingAlgorithm::SHELL_SORT);

	auto backBtn = tgui::Button::create("Back");
	backBtn->setSize({ "90%", 40 });
	backBtn->setPosition({ "5%", "90%" });
	backBtn->onClick([this] {
		this->algorithmPanel->setVisible(false);
		this->linkedListPanel->setVisible(false);
		this->selectedAlgo = SortingAlgorithm::NONE;
		});

	algorithmPanel->add(backBtn);

	gui.add(algorithmPanel);

	linkedListPanel = tgui::Panel::create({ "25%" ,"100%" });
	linkedListPanel->setPosition({ "25%","0%" });
	linkedListPanel->getRenderer()->setBackgroundColor(sf::Color(70, 70, 70));
	linkedListPanel->setVisible(false);

	auto linkedListLabel = tgui::Label::create("Choose Linked List Type");
	linkedListLabel->setTextSize(18);
	linkedListLabel->setPosition({ "5%","5%" });
	linkedListPanel->add(linkedListLabel);

	addLinkedListButton("SINGLE LINKED LIST", 15, LinkedListType::SINGLE);
	addLinkedListButton("DOUBLE LINKED LIST", 25, LinkedListType::DOUBLE);
	addLinkedListButton("CIRCULAR LINKED LIST", 35, LinkedListType::CIRCULAR);
	addLinkedListButton("DOUBLY CIRCULAR LINKED LIST", 45, LinkedListType::DOUBLE_CIRCULAR);

	auto LLBackBtn = tgui::Button::create("Back");
	LLBackBtn->setSize({ "90%", 40 });
	LLBackBtn->setPosition({ "5%", "90%" });
	LLBackBtn->onClick([this] {
		this->algorithmPanel->setVisible(false);
		this->linkedListPanel->setVisible(false);
		this->selectedLinkedList = LinkedListType::NONE;
		});

	linkedListPanel->add(LLBackBtn);

	gui.add(linkedListPanel);
}

void MenuUI::showAlgorithmPanel(){
	algorithmPanel->setVisible(true);
}

void MenuUI::hidePanels(){
	algorithmPanel->setVisible(false);
	linkedListPanel->setVisible(false);
	selectedAlgo = SortingAlgorithm::NONE;
}

SortingAlgorithm MenuUI::getSelectedAlgorithm(){
	return selectedAlgo;
}
