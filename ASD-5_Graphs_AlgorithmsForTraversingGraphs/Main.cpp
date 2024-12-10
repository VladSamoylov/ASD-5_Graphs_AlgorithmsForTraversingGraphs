#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <variant>

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_RED "\033[31m"

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
	void ShowNeighbor();
	variant<T, int> GetVertName() { return this->vertex; };
	vector<variant<T, int>> GetNeighbor();
	bool operator==(const Vertex& other) { return this->vertex == other.vertex; }
	bool operator==(const variant<T, int>& other) { return this->vertex == other; }
};

template <typename T>
void Vertex<T>::AddNeighbor(const variant<T, int>& vertex, const int& weight) {

	this->vertNeighbors.emplace_back(vertex, weight);
}

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

template <typename T>
vector<variant<T, int>> Vertex<T>::GetNeighbor() {

	vector<variant<T, int>> neighbour;
	for (auto v : this->vertNeighbors) {
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
	void DepthFirstSearch(const variant<T, int>&);
	void DFShelper(vector<variant<T, int>>&, const variant<T, int>&);
	void BreadthFirstSearch(const variant<T, int>&);
	void DepthFirstSearchStack(const variant<T, int>&);
	void BuildDFSForest(vector<variant<T, int>>&);
	void MinimumPathBetweenVerts(const variant<T, int>&, const variant<T, int>&);
	void Task2B(const variant<T, int>&);
};

template <typename T>
void Graph<T>::AddVertex(const variant<T, int>& vertex) {

	auto new_vertex = Vertex(vertex);
	if (find(this->listAdjacency.begin(), this->listAdjacency.end(), new_vertex) != this->listAdjacency.end()) return;
	this->listAdjacency.push_back(move(new_vertex));
	this->vertexN++;
}

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

template <typename T>
void Graph<T>::AddEdge(const variant<T, int>& fVertex, const variant<T, int>& sVertex, const int& weight) {

	this->AddVertex(fVertex);
	this->AddVertex(sVertex);

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
		visit([](auto&& val, auto&& val2) { cout << "The path from " << ANSI_COLOR_GREEN << val << ANSI_COLOR_RESET << " to " << ANSI_COLOR_BLUE << val2 << ANSI_COLOR_RESET << " : " << endl; }, sVert, eVert);
		while (!s.empty()) {
			visit([](auto&& val) {cout << val << "->"; }, s.top());
			s.pop();
		}cout << endl;
	}
	else visit([](auto&& val, auto&& val2) {cout << ANSI_COLOR_RED << "The path didn't find from " << val << " to " << val2 << ANSI_COLOR_RESET << endl; }, sVert, eVert);
}

template <typename T>
void Graph<T>::Task2B(const variant<T, int>& sVertex) {

	this->DepthFirstSearchStack(sVertex);

}

int main() {

	/*Graph<int> g(true);
	g.AddEdge(1, 2, 1);
	g.AddEdge(1, 3, 1);
	g.AddEdge(1, 4, 1);
	g.AddEdge(2, 3, 1);
	g.AddEdge(2, 5, 1);
	g.AddEdge(3, 5, 1);
	g.AddEdge(3, 6, 1);
	g.AddEdge(4, 3, 1);
	g.AddEdge(4, 6, 1);
	g.AddEdge(5, 4, 1);
	g.AddEdge(5, 6, 1);*/
	//g.ShowVertexes();
	
	Graph<string> g2(true);
	g2.AddEdge("A", "C", 1);
	g2.AddEdge("A", "F", 1);
	g2.AddEdge("C", "D", 1);
	g2.AddEdge("D", "B", 1);
	g2.AddEdge("E", "F", 1);
	g2.AddEdge("E", "5", 1);
	g2.ShowVertexes();

	try {
		//g.DepthFirstSearch(1);
		//g.DepthFirstSearch(5);
		//g.DepthFirstSearch(4);
		//g.DepthFirstSearch(2);
		g2.DepthFirstSearch("C");
		g2.DepthFirstSearchStack("C");
		//g.DepthFirstSearchStack(1);
		//g.BreadthFirstSearch(1);
		g2.BreadthFirstSearch("A");
		g2.MinimumPathBetweenVerts("A", "D");
		g2.Task2B("A");
	}
	catch (const char* err) {
		cerr << err << endl;
	}
	

	return 0;
}