#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <variant>
#include <fstream>
#include <string>
#include <iomanip>

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_PURPLE "\033[35m"
#define ANSI_COLOR_LIGHTBLUE "\033[94m"

using namespace std;

template <typename T>
class Graph;

template <typename T>
class Vertex {
private:
	variant<T, int> vertex;
	vector<pair<variant<T, int>, int>> vertNeighbors;
public:
	Vertex(const variant<T, int>& vertex) : vertex(vertex), vertNeighbors() {};
	void AddNeighbor(const variant<T, int>&, const int&);
	void SetNameNeighbor(const vector<variant<T, int>>&);
	void ShowNeighbor();
	variant<T, int> GetVertName() { return this->vertex; };
	void SetVertName(const variant<T, int>& newName) { this->vertex = newName; };
	vector<variant<T, int>> GetNeighbor();
	bool operator==(const Vertex& other) { return this->vertex == other.vertex; }
	bool operator==(const variant<T, int>& other) { return this->vertex == other; }
};

/**
 * @brief Метод додавання суміжної вершини
 * @tparam T Заповнювач типу даних
 * @param vertex Назва суміжної вершини
 * @param weight Вага суміжної вершини
 */
template <typename T>
void Vertex<T>::AddNeighbor(const variant<T, int>& vertex, const int& weight) {

	this->vertNeighbors.emplace_back(vertex, weight);
}

/**
 * @brief Метод зміни назв суміжних вершин
 * @tparam T Заповнювач типу даних
 * @param newName Нові ім'я суміжних вершин
 */
template <typename T>
void Vertex<T>::SetNameNeighbor(const vector<variant<T, int>>& newName) {

	if (this->vertNeighbors.size() != newName.size()) throw "Error: <Amount neighbours != new amount>";

	for (int i = 0; i < this->vertNeighbors.size(); i++) {
		this->vertNeighbors[i].first = newName[i];
	}
}

/**
 * @brief Метод виведення суміжних вершин
 * @tparam T Заповнювач типу даних
 */
template <typename T>
void Vertex<T>::ShowNeighbor() {

	cout << "vertex ";
	visit([](auto&& val) { cout << val; }, this->vertex);
	cout << " : ";
	for (auto v : this->vertNeighbors) {
		visit([](auto&& val) { cout << val; }, v.first);
		cout << "(" << v.second << ") ";
	}cout << endl;
}

/**
 * @brief Метод отримання списку суміжних вершин
 * @tparam T Заповнювач типу даних
 * @return Список суміжних вершин
 */
template <typename T>
vector<variant<T, int>> Vertex<T>::GetNeighbor() {

	vector<variant<T, int>> neighbour;
	for (auto& v : this->vertNeighbors) {
		neighbour.push_back(v.first);
	}
	return neighbour;
}

template <typename T>
class Graph {
private:
	bool isOriented;
	int vertexN;
	vector<Vertex<T>> listAdjacency;
public:
	Graph() : isOriented(false), vertexN(0), listAdjacency() {};
	Graph(const bool& isOriented) : isOriented(isOriented), vertexN(0), listAdjacency() {};
	void AddEdge(const variant<T, int>&, const variant<T, int>&, const int&);
	void AddVertex(const variant<T, int>&);
	void ShowVertexes();
	int GetAmountVertex() { return this->vertexN; }
	bool isPresentEdge(const variant<T, int>&, const variant<T, int>&);
	vector<Vertex<T>> GetListAdjacency() { return this->listAdjacency; }
	void DepthFirstSearch(const variant<T, int>&);
	void DFShelper(vector<variant<T, int>>&, const variant<T, int>&);
	void BreadthFirstSearch(const variant<T, int>&);
	void DepthFirstSearchStack(const variant<T, int>&);
	void BuildDFSForest(vector<variant<T, int>>&);
	void MinimumPathBetweenVerts(const variant<T, int>&, const variant<T, int>&);
	void Task2B(const variant<T, int>&);
	void ClearDataGraph() { this->vertexN = 0; this->listAdjacency.clear(); }
};

/**
 * @brief Метод додавання вершини
 * @tparam T Заповнювач типу даних
 * @param vertex Ім'я вершини
 */
template <typename T>
void Graph<T>::AddVertex(const variant<T, int>& vertex) {

	auto new_vertex = Vertex(vertex);
	if (find(this->listAdjacency.begin(), this->listAdjacency.end(), new_vertex) != this->listAdjacency.end()) return;
	this->listAdjacency.push_back(move(new_vertex));
	this->vertexN++;
}

/**
 * @brief Метод перевірки існування ребра графа
 * @tparam T Заповнювач типу даних
 * @param fVert Перша вершина
 * @param sVert Друга вершина
 * @return True ребро існує, інакше - false
 */
template <typename T>
bool Graph<T>::isPresentEdge(const variant<T, int>& fVert, const variant<T, int>& sVert) {

	auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), fVert);
	if (it != this->listAdjacency.end()) {
		vector<variant<T, int>> neighbours = it->GetNeighbor();
		auto isConsist = find(neighbours.begin(), neighbours.end(), sVert);
		if (isConsist != neighbours.end()) return true;
	}

	return false;
}

/**
 * @brief Метод відображення списку суміжності
 * @tparam T Заповнювач типу даних
 */
template <typename T>
void Graph<T>::ShowVertexes() {

	cout << "The Graph (";
	if (isOriented) cout << "oriented";
	else cout << "non-oriented";
	cout << ") has: " << endl;
	for (auto i : this->listAdjacency) {
		i.ShowNeighbor();
	}cout << endl;
}

/**
 * @brief Метод додавання ребра графа
 * @tparam T Заповнювач типу даних
 * @param fVertex Перша вершина
 * @param sVertex Друга вершина
 * @param weight Вага ребра
 */
template <typename T>
void Graph<T>::AddEdge(const variant<T, int>& fVertex, const variant<T, int>& sVertex, const int& weight) {

	this->AddVertex(fVertex);
	this->AddVertex(sVertex);

	if (this->isPresentEdge(fVertex, sVertex)) return;
	auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), fVertex);
	if (it != this->listAdjacency.end()) {
		it->AddNeighbor(sVertex, weight);
	}
	if (!this->isOriented) {
		it = find(this->listAdjacency.begin(), this->listAdjacency.end(), sVertex);
		if (it != this->listAdjacency.end()) {
			it->AddNeighbor(fVertex, weight);
		}
	}
}

/**
 * @brief Метод пошуку в глибину з використанням рекурсії
 * @tparam T Заповнювач типу даних
 * @param sVertex Початкова вершина
 */
template <typename T>
void Graph<T>::DepthFirstSearch(const variant<T, int>& sVertex) {

	vector<variant<T, int>> visitedVerts;
	this->DFShelper(visitedVerts, sVertex);
	cout << "Sequence of traversal of vertices in a graph (by DFS) : " << endl;
	for (auto travel : visitedVerts) {
		visit([](auto&& val) { cout << val << "->"; }, travel);
	}cout << endl;
	this->BuildDFSForest(visitedVerts);
}

/**
 * @brief Метод рекурсія для пошуку в глибину
 * @tparam T Заповнювач типу даних
 * @param visited Список відвіданих вершин
 * @param sVertex Початкова вершина
 */
template <typename T>
void Graph<T>::DFShelper(vector<variant<T, int>>& visited, const variant<T, int>& sVertex) {

	auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), sVertex);
	if (it != this->listAdjacency.end()) {
		visited.push_back(sVertex);
		for (auto neighbour : it->GetNeighbor()) {
			auto isConsist = find(visited.begin(), visited.end(), neighbour);
			if (isConsist == visited.end()) {
				this->DFShelper(visited, neighbour);
			}
		}
	}
	else throw "Error: <The current Vertex doesn't contain in the Graph>";
}

/**
 * @brief Метод пошуку в ширину і побудова дерева пошибку в глибину
 * @tparam T Заповнювач типу даних 
 * @param sVertex Початкова вершина
 */
template <typename T>
void Graph<T>::BreadthFirstSearch(const variant<T, int>& sVertex) {

	vector<variant<T, int>> visitedVerts;
	queue<variant<T, int>> q;
	variant<T, int> currentElem;
	unordered_map<variant<T, int>, vector<variant<T, int>>> bfsTree;

	q.push(sVertex);
	visitedVerts.push_back(sVertex);
	while (!q.empty()) {
		currentElem = q.front();
		q.pop();
		auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), currentElem);
		if (it != this->listAdjacency.end()) {
			for (auto neighbour : it->GetNeighbor()) {
				auto isConsist = find(visitedVerts.begin(), visitedVerts.end(), neighbour);
				if (isConsist == visitedVerts.end()) {
					q.push(neighbour);
					visitedVerts.push_back(neighbour);
					bfsTree[currentElem].push_back(neighbour);
				}
			}
		}
		else throw "Error: <The current Vertex doesn't contain in the Graph>";
	}
	cout << "Sequence of traversal of vertices in a graph (by BFS) : " << endl;
	for (auto travel : visitedVerts) {
		visit([](auto&& val) {cout << val << "->"; }, travel);
	}cout << endl;
	cout << "BFS Tree: " << endl;
	for (auto [parent, children] : bfsTree) {
		visit([](auto&& val) {cout << val << ": "; }, parent);		
		for (auto child : children) {
			visit([](auto&& val) {cout << val << " "; }, child);
		}cout << endl;
	}
}

/**
 * @brief Метод пошуку в глибину з використанням структури даних стек
 * @tparam T Заповнювач типу даних 
 * @param sVertex Початкова вершина
 */
template <typename T>
void Graph<T>::DepthFirstSearchStack(const variant<T, int>& sVertex) {

	vector<variant<T, int>> visitedVerts;
	stack<variant<T, int>> s;	
	variant<T, int> currentElem;
	s.push(sVertex);
	while (!s.empty()) {
		currentElem = s.top();
		s.pop();
		auto isVisited = find(visitedVerts.begin(), visitedVerts.end(), currentElem);
		if (isVisited == visitedVerts.end()) {
			visitedVerts.push_back(currentElem);
			auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), currentElem);
			if (it != this->listAdjacency.end()) {
				vector<variant<T, int>> neighbours = it->GetNeighbor();
				reverse(neighbours.begin(), neighbours.end());
				for (auto& neighbour : neighbours) {
					auto isConsist = find(visitedVerts.begin(), visitedVerts.end(), neighbour);
					if (isConsist == visitedVerts.end()) {
						s.push(neighbour);
					}
				}
			}
			else throw "Error: <The current Vertex doesn't contain in the Graph>";
		}
	}
	cout << "Sequence of traversal of vertices in a graph (by DFS_stack) : " << endl;
	for (auto travel : visitedVerts) {
		visit([](auto&& val) {cout << val << "->"; }, travel);
	}cout << endl;
	this->BuildDFSForest(visitedVerts);
}

/**
 * @brief Метод побудови лісу пошуку в глибину
 * @tparam T Заповнювач типу даних
 * @param visitedVerts Список відвіданих вершин
 */
template <typename T>
void Graph<T>::BuildDFSForest(vector<variant<T, int>>& visitedVerts) {

	vector<vector<variant<T, int>>> dfsForest;
	dfsForest.push_back(visitedVerts);
	for (auto v : this->listAdjacency) {
		auto finder = find(visitedVerts.begin(), visitedVerts.end(), v.GetVertName());
		if (finder == visitedVerts.end()) {
			vector<variant<T, int>> difVisitedVerts(visitedVerts);
			DFShelper(difVisitedVerts, v.GetVertName());
			unordered_set<variant<T, int>> set2(visitedVerts.begin(), visitedVerts.end());
			difVisitedVerts.erase(remove_if(difVisitedVerts.begin(), difVisitedVerts.end(), [&set2](variant<T, int> x) { return set2.find(x) != set2.end(); }), difVisitedVerts.end());
			dfsForest.push_back(difVisitedVerts);
			for (auto i : difVisitedVerts) { visitedVerts.push_back(i); }
		}
	}
	cout << "DFS Forecast:" << endl;
	for (auto tree : dfsForest) {
		cout << "Tree: ";
		for (auto v : tree) {
			visit([](auto&& val) {cout << val << " "; }, v);
		}cout << endl;
	}	
}

/**
 * @brief Метод пошуку найкоротшого шляху (модифікація хвильового алгоритму)
 * @tparam T Заповнювач типу даних
 * @param sVert Початкова вершина
 * @param eVert Кінцева вершина
 */
template <typename T>
void Graph<T>::MinimumPathBetweenVerts(const variant<T, int>& sVert, const variant<T, int>& eVert) {

	if (sVert == eVert) throw "Error: <Start vert and End vert is equal>";
	vector<variant<T, int>> visitedVerts;
	queue<variant<T, int>> q;
	variant<T, int> currentElem;
	unordered_map<variant<T, int>, variant<T, int>> pathParrent;
	
	q.push(sVert);
	visitedVerts.push_back(sVert);
	while (!q.empty()) {
		currentElem = q.front();
		q.pop();
		if (currentElem == eVert) break;
		auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), currentElem);
		if (it != this->listAdjacency.end()) {
			for (auto neighbour : it->GetNeighbor()) {
				auto isConsist = find(visitedVerts.begin(), visitedVerts.end(), neighbour);
				if (isConsist == visitedVerts.end()) {
					q.push(neighbour);
					visitedVerts.push_back(neighbour);
					pathParrent[neighbour] = currentElem;
				}
			}
		}
		else throw "Error: <The current Vertex doesn't contain in the Graph>";
	}
	if (pathParrent.find(eVert) != pathParrent.end()) {
		stack<variant<T, int>> s;
		currentElem = eVert;
		while (true) {
			s.push(currentElem);
			if (currentElem == sVert) break;
			currentElem = pathParrent[currentElem];
		}
		cout << "The path lenght is " << s.size() - 1 << endl;
		visit([](auto&& val, auto&& val2) { cout << "The path from " << ANSI_COLOR_GREEN << val << ANSI_COLOR_RESET << " to " << ANSI_COLOR_LIGHTBLUE << val2 << ANSI_COLOR_RESET << " : " << endl; }, sVert, eVert);
		while (!s.empty()) {
			visit([](auto&& val) {cout << ANSI_COLOR_PURPLE << val << ANSI_COLOR_RESET << "->"; }, s.top());
			s.pop();
		}cout << endl;
	}
	else visit([](auto&& val, auto&& val2) {cout << ANSI_COLOR_RED << "The path didn't find from " << val << " to " << val2 << ANSI_COLOR_RESET << endl; }, sVert, eVert);
}

/**
 * @brief Метод променумерування вершин у порядку обходу в глибину
 * @tparam T Заповнювач типу даних
 * @param sVertex Початкова вершина
 */
template <typename T>
void Graph<T>::Task2B(const variant<T, int>& sVertex) {

	this->ShowVertexes();
	vector<variant<T, int>> visitedVerts;
	unordered_map <variant<T, int>, variant<T, int>> newNameV;
	int edgeN = 0;
	this->DFShelper(visitedVerts, sVertex);
	cout << "Sequence of traversal of vertices in a graph (by DFS) : " << endl;
	for (auto& travel : visitedVerts) {
		visit([](auto&& val) { cout << val << "->"; }, travel);
	}cout << endl;
	for (auto& v : this->listAdjacency) {
		newNameV[v.GetVertName()] = v.GetVertName();
		vector<variant<T, int>> neighbours = v.GetNeighbor();
		for (auto& neighbour : neighbours) {
			auto itV = find(visitedVerts.begin(), visitedVerts.end(), v.GetVertName());
			auto itN = find(visitedVerts.begin(), visitedVerts.end(), neighbour);
			if (itV != visitedVerts.end() && itN != visitedVerts.end()) edgeN++;
		}
	}
	for (int i = 0; i < visitedVerts.size(); i++) {
		auto it = find(this->listAdjacency.begin(), this->listAdjacency.end(), visitedVerts[i]);
		if (it != this->listAdjacency.end()) {
			newNameV[it->GetVertName()] = i + 1;
		}
	}
	for (auto& vertex : this->listAdjacency) {
		vector<variant<T, int>> neighbours = vertex.GetNeighbor();
		for (auto& neighbour : neighbours) {
			neighbour = newNameV[neighbour];
		}
		if (newNameV.find(vertex.GetVertName()) != newNameV.end()) { 
			vertex.SetVertName(newNameV[vertex.GetVertName()]);
		}
		vertex.SetNameNeighbor(neighbours);
	}
	this->ShowVertexes();
	cout << "Amount verteces of Graph: " << visitedVerts.size() << endl;
	if (!this->isOriented) edgeN /= 2;
	cout << "Amount edge of Graph: " << edgeN << endl;	
	//double density = edgeN / visitedVerts.size();
	double density = 1;
	if (visitedVerts.size() <= 1) throw "Error: <The density cannot be calculated (not enough vertices)>";
	density = static_cast<double>(edgeN) / (visitedVerts.size() * (visitedVerts.size() - 1));
	if (!isOriented) density = 2.0 * static_cast<double>(edgeN) / (visitedVerts.size() * (visitedVerts.size() - 1));
	cout << "The graph density : " << density << endl;
}

class ConvertAtoB {
private:
	Graph<string> graph;
public:
	ConvertAtoB() : graph(true) {}
	unordered_set<string> LoadFromDictionary(const string&, const int&);
	void MakeTheTransformation(const string&, const string&);
	void MakeGraph(const string&, const string&, const unordered_set<string>&);
	void ShowGraph() { this->graph.ShowVertexes(); }
};

/**
 * @brief Метод отримання слів зі словника
 * @param pathDic Шлях до словника
 * @param size Розмір слова
 * @return Знайдені слова зі словника 
 */
unordered_set<string> ConvertAtoB::LoadFromDictionary(const string& pathDic, const int& size) {

	ifstream fileDic(pathDic);
	if (!fileDic) throw "Error: <The dictionary isn't find>";
	unordered_set<string> words;
	string line;
	getline(fileDic, line);
	while (getline(fileDic, line)) {
		if (!line.empty()) {
			auto pos = line.find('/');
			if (pos != string::npos) line = line.substr(0, pos);
		}
		if (line.size() == size) words.insert(line);
	}
	fileDic.close();
	cout << "Find " << words.size() << " Words" << endl;

	return words;
}

/**
 * @brief Метод перетворення слова А у слово B
 * @param A Слово А
 * @param B Слово В
 */
void ConvertAtoB::MakeTheTransformation(const string& A, const string& B) {

	if (A.size() != B.size()) throw "Error: <A and B sizes aren't equal>";
	string path = "Dictionary/en_GB.dic";
	unordered_set<string> words = this->LoadFromDictionary(path, A.size());
	auto itA = find(words.begin(), words.end(), A);
	if (itA == words.end()) throw "Error: <The word A is unknown>";
	auto itB = find(words.begin(), words.end(), B);
	if (itB == words.end()) throw "Error: <The word B is unknown>";
	cout << "From " << ANSI_COLOR_LIGHTBLUE << A << ANSI_COLOR_RESET << " to " << ANSI_COLOR_GREEN << B << ANSI_COLOR_RESET << endl;
	this->MakeGraph(A, B, words);
	cout << "The graph is consist " << ANSI_COLOR_LIGHTBLUE << this->graph.GetAmountVertex() << ANSI_COLOR_RESET << " Verteces" << endl;
	this->graph.MinimumPathBetweenVerts(A, B);
}

void ConvertAtoB::MakeGraph(const string& A, const string& B, const unordered_set<string>& words) {
	
	string newWord = A, oldWord = A;
	queue<string> q;
	unordered_set<string> uniqueName;
	this->graph.ClearDataGraph();
	bool findB = false;
	q.push(A);
	while (!findB && !q.empty()) {
		oldWord = q.front();
		q.pop();
		for (int i = 0; i < A.size(); i++) {
			for (char b = 'a'; b <= 'z'; b++) {
				newWord = oldWord;
				if (newWord[i] == b) continue;
				newWord[i] = b;
				if (words.count(newWord) && !uniqueName.count(newWord)) {
					//cout << newWord << endl;
					this->graph.AddEdge(oldWord, newWord, 1);
					q.push(newWord);
					uniqueName.insert(newWord);
					if (newWord == B) { findB = true; cout << ANSI_COLOR_PURPLE << "The word B is found" << ANSI_COLOR_RESET << endl; }
				}
			}
		}
	}
	if (!findB) {
		cout << ANSI_COLOR_RED << "<No found by replace just one letter!>" << ANSI_COLOR_RESET << ANSI_COLOR_LIGHTBLUE << " Let's try replace 2 letters" << ANSI_COLOR_RESET << endl;
	}
	else return;
	uniqueName.clear();
	q.push(A);
	while (!findB && !q.empty()) {
		oldWord = q.front();
		q.pop();
		for (int i = 0; i < A.size(); i++) {
			for (int j = i + 1; j < A.size(); j++) {
				for (char b = 'a'; b <= 'z'; b++) {
					for (char b2 = 'a'; b2 <= 'z'; b2++) {
						newWord = oldWord;
						if (newWord[i] == b && newWord[j] == b2) continue;
						newWord[i] = b;
						newWord[j] = b2;
						if (words.count(newWord) && !uniqueName.count(newWord)) {
							//cout << newWord << endl;
							this->graph.AddEdge(oldWord, newWord, 1);
							q.push(newWord);
							uniqueName.insert(newWord);
							if (newWord == B) { findB = true; cout << ANSI_COLOR_PURPLE << "The word B is found" << ANSI_COLOR_RESET << endl; }
						}
					}
				}
			}
		}
	}
	if (!findB) {
		cout << ANSI_COLOR_RED << "No transformation path found from A to B." << ANSI_COLOR_RESET << endl;
	}
}

int main() {

	srand(time(0));
	int task = 99, ltask = 99, vtask = 99;
	bool isOrinted = true;
	string A, B;
	Graph<string> g(true), g2(false);
	ConvertAtoB c;

	while (task != 0) {
		switch (task) {
		case 1:
			while (ltask != 0) {
				switch (ltask) {
				case 1:
					g.ClearDataGraph();
					isOrinted = true;
					g.AddEdge("A", "B", 1);
					g.AddEdge("A", "C", 1);
					g.AddEdge("A", "D", 1);
					g.AddEdge("B", "E", 1);
					g.AddEdge("D", "E", 1);
					g.AddEdge("C", "F", 1);
					g.AddEdge("E", "G", 1);
					g.AddEdge("F", "G", 1);
					g.AddEdge("Y", "X", 1);
					break;
				case 2:
					g2.ClearDataGraph();
					isOrinted = false;
					g2.AddEdge("A", "B", 1);
					g2.AddEdge("A", "C", 1);
					g2.AddEdge("A", "D", 1);
					g2.AddEdge("B", "E", 1);
					g2.AddEdge("C", "F", 1);
					g2.AddEdge("E", "G", 1);
					g2.AddEdge("F", "G", 1);
					g2.AddEdge("X", "Y", 1);
					break;
				case 3:
					g.ClearDataGraph();
					g2.ClearDataGraph();
					break;
				case 4:
					if(isOrinted) g.ShowVertexes();
					else g2.ShowVertexes();
					break;
				case 5:
					try {
						if (isOrinted) g.DepthFirstSearch("A");
						else g2.DepthFirstSearch("A");
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 6:
					try {
						if(isOrinted) g.DepthFirstSearchStack("A");
						else g2.DepthFirstSearchStack("A");
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 7:
					try {
						if(isOrinted) g.BreadthFirstSearch("A");
						else g2.BreadthFirstSearch("A");
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				}
				cout << "\n________Work with Graph________" << endl;
				cout << setw(35) << left << "Build and shoose orinted graph" << " / Enter - 1" << endl;
				cout << setw(35) << left << "Build and shoose non-orinted graph " << " / Enter - 2" << endl;
				cout << setw(35) << left << "Clear graph " << " / Enter - 3" << endl;
				cout << setw(35) << left << "Show Graph " << " / Enter - 4" << endl;
				cout << setw(35) << left << "Launch DFS" << " / Enter - 5" << endl;
				cout << setw(35) << left << "Launch DFS by Stack" << " / Enter - 6" << endl;
				cout << setw(35) << left << "Launch BFS" << " / Enter - 7" << endl;
				cout << setw(35) << left << "Back " << " / Enter - 0" << endl;
				if (ltask != 0) {
					try {
						cout << "\nChoose the Task: ";
						cin >> ltask;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							throw "Error: <Incorrect Input Data>";
						}
					}
					catch (const char* err) {
						cerr << endl << err << endl;
						ltask = 99;
					}
				}
			}
			break;
		case 2:
			while (vtask != 0) {
				switch (vtask) {
				case 1:
					try {
						cout << "Enter first Vertex : ";
						cin >> A;
						cout << "Enter second Vertex : ";
						cin >> B;
						if (isOrinted) g.MinimumPathBetweenVerts(A, B);
						else g2.MinimumPathBetweenVerts(A, B);
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 2:
					try {
						cout << "Enter start Vertex : ";
						cin >> A;
						if (isOrinted) g.Task2B(A);
						else g2.Task2B(A);
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 3:
					try {
						cout << "Enter first Word : ";
						cin >> A;
						cout << "Enter second Word : ";
						cin >> B;
						c.MakeTheTransformation(A, B);
						cout << "Show the Graph? 1/0 :";
						int k;
						cin >> k;
						if (k == 1) c.ShowGraph();
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				}
				cout << "\n________Work with Lab Task var 17________" << endl;
				cout << setw(35) << left << "Find min path from A to B (G) " << " / Enter - 1" << endl;
				cout << setw(35) << left << "Number the vertices by DFS (B) " << " / Enter - 2" << endl;
				cout << setw(35) << left << "Convert word from A to B " << " / Enter - 3" << endl;
				cout << setw(35) << left << "Back " << " / Enter - 0" << endl;
				if (vtask != 0) {
					try {
						cout << "\nChoose the Task: ";
						cin >> vtask;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							throw "Error: <Incorrect Input Data>";
						}
					}
					catch (const char* err) {
						cerr << endl << err << endl;
						vtask = 99;
					}
				}				
			}
			break;
		case 0:
			break;
		}
		cout << "\n________________Menu of action________________" << endl;
		cout << setw(35) << left << "Work with graph " << " / Enter - 1" << endl;
		cout << setw(35) << left << "Work with Lab Task var 17  " << " / Enter - 2" << endl;
		cout << setw(35) << left << "Exit " << " / Enter - 0" << endl;

		if (task != 0) {
			ltask = 99;
			vtask = 99;
			try {
				cout << "\nChoose the Task: ";
				cin >> task;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					throw "Error: <Incorrect Input Data>";
				}
			}
			catch (const char* err) {
				cerr << endl << err << endl;
				task = 99;
			}
		}
	}

	//c.MakeTheTransformation("cat", "dog");
	//c.MakeTheTransformation("doggie", "kitten");
	//c.MakeTheTransformation("marriage", "superior");
	//c.MakeTheTransformation("great", "adept");
	//c.MakeTheTransformation("tree", "grey");
	//c.MakeTheTransformation("make", "girl");
	//c.MakeTheTransformation("marital", "thunder");
	//c.MakeTheTransformation("app", "two");
	//c.MakeTheTransformation("gap", "bag");
	return 0;
}