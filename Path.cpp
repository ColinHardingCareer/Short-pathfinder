// Path.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "IndexedPQ.h"
#include <fstream>
#include <unordered_map>
#include <stack>
#include <math.h>
using namespace std;


class Edge;
class Vertex {
public:

	Vertex(char n) {
		name = n;
		mark = false;
	}

	bool mark;
	bool inFringe;
	double priority;
	Edge* parent;
	vector<Edge*> neighbors;
	int idx;
	char name;

	friend ostream& operator<<(ostream& os, const Vertex& p) {
		os << p.name << " " << p.priority;
		return os;
	}
	bool operator> (const Vertex& b) const {
		return idx > b.idx || (idx == b.idx && idx > b.idx);
	}

	void setMark(bool b) {
		mark = b;
	}
	bool getMark() {
		return mark;
	}

	void setInFringe(bool b) {
		inFringe = b;
	}
	bool getInFringe() {
		return inFringe;
	}

	void setPriority(double d) {
		priority = d;
	}
	double getPriority() {
		return priority;
	}

	void setParentEdge(Edge* e) {
		parent = e;
	}
	Edge* getParentEdge() {
		return parent;
	}

	vector<Edge*> getNeighbors() {
		return neighbors;
	}
	void addNeighbor(Edge* e) {
		neighbors.push_back(e);
	}
};
class Edge {
public:
	Edge(Vertex* v, double d) {
		vertex = v;
		weight = d;
	}

	Vertex* vertex;
	double weight;

	Vertex* getV() {
		return vertex;
	}
	double getD() {
		return weight;
	}
};
void visit(Vertex* v, IndexedPQ<Vertex>& pq) {
	v->setMark(true);
	for (Edge* p : v->getNeighbors()) {
		Vertex* x = p->getV();
		if (x->getMark())
			continue;
		if (!x->getInFringe() || x->getPriority() > v->getPriority() + p->getD()) {
			x->setPriority(v->getPriority() + p->getD());

			x->setParentEdge(new Edge(v, p->getD()));
			if (!x->getInFringe())
				pq.insert(x);
			else
				pq.change(x);
			x->setInFringe(true);
		}
	}
}

Vertex* findSP(Vertex* s, Vertex* d) {
	IndexedPQ<Vertex> pq;
	//pq.delMax();
	s->setPriority(0);
	visit(s, pq);
	while (!pq.empty()) {
		Vertex* v = pq.delMax();
		if (v == d) {
			return v;
		}
		v->setInFringe(false);
		visit(v, pq);
	}
}





int main()
{
	unordered_map<char,Vertex*> SnS;

	fstream myfile;
	myfile.open("input.txt");
	char s, d;
	myfile >> s >> d;
	SnS[s] = new Vertex(s);
	SnS[d] = new Vertex(d);

	vector<Edge*> edges;

	char v,w;
	double h;
	while (myfile >> v) {
		myfile >> w;
		myfile >> h;
		Vertex* tv = new Vertex(v);
		Vertex* tw = new Vertex(w);
		
		if (SnS.count(v) == 1 && SnS.count(w) == 0) {
			SnS[w] = tw;

		}
		else if (SnS.count(v) == 0 && SnS.count(w) == 1) {
			SnS[v] = tv;

		}
		else if (SnS.count(v) == 0 && SnS.count(w) == 0) {
			SnS[w] = tw;
			SnS[v] = tv;

		}
		SnS[v]->addNeighbor(new Edge(SnS[w], h));
		SnS[w]->addNeighbor(new Edge(SnS[v], h));

	}
	Vertex* f = findSP(SnS[s], SnS[d]);
	stack<Vertex*> stack;
	
	while (f->getParentEdge() != NULL) {
		stack.push(f);
		f = f->getParentEdge()->getV();
	}
	stack.push(f);
	double prob = 1;
	double probl = 0;
	cout << "Path:" << endl;
	while (!stack.empty()) {
		cout << *stack.top() << endl;
		probl += log(1 - stack.top()->getPriority());
		stack.pop();
	}

	cout << "Probability of success: " << exp(probl) << endl;

}

