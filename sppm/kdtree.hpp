#ifndef __Kdtree_H__
#define __Kdtree_H__ 

#include "obj.hpp"
//#include "bezier.hpp"




class Kdtree
{
public:
	static int D;
	int n, root;
	struct KdtreeNode {
		noderec sppm;
		vec3 m[2];
		int s[2];
		KdtreeNode(): sppm() {s[0] = s[1] = 0;}
		bool operator<(const KdtreeNode&a) const {
			if (D == 0)
				return sppm.pos.x < a.sppm.pos.x;
			else if (D == 1)
				return sppm.pos.y < a.sppm.pos.y;
			else
				return sppm.pos.z < a.sppm.pos.z;
		}
	};
	KdtreeNode* tree;
	Kdtree() { tree = NULL; }
	~Kdtree() { if (tree != NULL) delete[] tree; }
	void mt(int f, int x) {
		tree[f].m[0] = tree[f].m[0].min(tree[x].m[0]);
		tree[f].m[1] = tree[f].m[1].max(tree[x].m[1]);
	}
	int bt(int l, int r, int d) {
		D = d;
		int o = l + r >> 1;
		std::nth_element(tree + l, tree + o, tree + r + 1);
		tree[o].m[0] = tree[o].sppm.pos - tree[o].sppm.r;
		tree[o].m[1] = tree[o].sppm.pos + tree[o].sppm.r;

		if (l < o) tree[o].s[0] = bt(l, o - 1, d == 2 ? 0 : d + 1), mt(o, tree[o].s[0]);
		if (o < r) tree[o].s[1] = bt(o + 1, r, d == 2 ? 0 : d + 1), mt(o, tree[o].s[1]);

		return o;
	}
	Kdtree(std::vector<noderec>& node) { // multi-thread forbid !!!
		init(node);
	}
	void init(std::vector<noderec>& node) { // multi-thread forbid !!!
		n = node.size();
		if (tree != NULL) delete[] tree;
		tree = new KdtreeNode[n + 10];
		for (int i = 0; i < n; ++i) {
			tree[i + 1].sppm = node[i];
		}
		root = bt(1, n, 0);
	}
	double getdis2(vec3 pos, vec3 m0, vec3 m1) {
		return (vec3().max(pos - m1).max(m0 - pos)).len2();
	}
	void _query(const noderec&node, vec2* c, int o) {
		if ((tree[o].sppm.pos - node.pos).len2() <= sqr(tree[o].sppm.r) && tree[o].sppm.dir.dot(node.dir) >= 0)
			c[tree[o].sppm.index].add(node.col.com(tree[o].sppm.col), node.prob);
		double d[2];
		if (tree[o].s[0] > 0) d[0] = getdis2(node.pos, tree[tree[o].s[0]].m[0], tree[tree[o].s[0]].m[1]); 
		else d[0] = INF;
		if (tree[o].s[1] > 0) d[1] = getdis2(node.pos, tree[tree[o].s[1]].m[0], tree[tree[o].s[1]].m[1]); 
		else d[1] = INF;
		int tmp = d[0] >= d[1];
		if (d[tmp] < 1e-6) _query(node, c, tree[o].s[tmp]); tmp ^= 1;
		if (d[tmp] < 1e-6) _query(node, c, tree[o].s[tmp]);
	}
	void _modify(int o) {
		tree[o].m[0] = tree[o].sppm.pos - tree[o].sppm.r;
		tree[o].m[1] = tree[o].sppm.pos + tree[o].sppm.r;
		if (tree[o].s[0] > 0) _modify(tree[o].s[0]), mt(o, tree[o].s[0]);
		if (tree[o].s[1] > 0) _modify(tree[o].s[1]), mt(o, tree[o].s[1]);
	}
	void query(noderec node, vec2* c) {
		_query(node, c, root);
	}
	void modify() {
		_modify(root);
	}
};

int Kdtree::D = 0;

#endif // __Kdtree_H__