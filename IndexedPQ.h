#ifndef INDEXEDPQHDR
#define INDEXEDPQHDR

#include <iostream>
#include <vector>

using namespace std;

template <class Key>
class IndexedPQ {
  // Min Priority Queue with an index, allowing for a change function.
  // Key class must have a public field "int idx" which is used only by IndexedPQ.
  // Key class must override operator> to compare priorities.
  // Key class must override operator<< to print out.

private: 
  vector<Key*> pq;
  int N = 0;    

public: 
  IndexedPQ() {
    pq.insert(pq.begin(), NULL);
  }

  IndexedPQ(Key* keys[], int n) {
    pq.insert(pq.begin(), NULL);
    pq.insert(pq.end(), keys, keys+n);
    N = n;
    for (int k = N/2; k >= 1; k--)
      sink(k);
    for(int k = 1; k <= N; k++)
      pq[k]->idx = k;
  }

  template <class X>
  friend ostream& operator<<(ostream &os, const IndexedPQ<X>& p) {
    for(auto q = p.pq.begin()+1; q < p.pq.end(); q++)
      os << **q << " ";
    return os;
  }

  void swim(int k) {
    Key *x = pq[k];
    while (k > 1 && *(pq[k/2]) > *x) {
      pq[k] = pq[k/2];
      pq[k]->idx = k;
      k = k/2;
    }
    pq[k] = x;
    x->idx = k;
  }

  void sink(int k) {
    Key *x = pq[k];
    while (2*k <= N) {
      int j = 2*k;
      if (j<N && *(pq[j]) > *(pq[j+1]))
	j++;
      if (!(*x > *(pq[j]))) break;
      pq[k] = pq[j];
      pq[k]->idx = k;
      k = j;
    }
    pq[k] = x;
    x->idx = k;
  }

  bool empty() { return N == 0; }

  int size() { return N; }

  Key* max() { return pq[1];}

  void insert(Key* x) {
    N++;
    pq.insert(pq.end(),x);
    x->idx = N;
    swim(N);
  }

  Key* delMax() {
    Key *max = pq[1];
    Key *t = pq[1]; pq[1] = pq[N]; pq[N] = t;
    pq[1]->idx = 1; pq[N]->idx = N;
    N--;
    sink(1);
    pq.erase(pq.end()-1);
    return max;
  }

  void change(Key* x) {
    int k = x->idx;
    pq[k] = x;
    swim(k);
    sink(k);
  }
};

#endif

