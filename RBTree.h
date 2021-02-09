#pragma once

#include <memory>
#include <functional>
#include <iostream>
#include <assert.h>

typedef enum { 
	RED = 0,
	BLACK
} Color;

template<typename T>
struct RBTNode {
	Color color;
	T key;
	RBTNode* left;
	RBTNode* right;
	RBTNode* parent;
};

template<typename T>
class RBTree
{
public:
	using node_t = RBTNode<T>;
public:
	RBTree() {
		nil = buyNode();
		root = nil;
		nil->color = BLACK;
	}

	~RBTree() {
		destroy(root);
		delete nil;
		nil = nullptr;
	}

	/// <summary>
	/// 中序遍历
	/// </summary>
	void inOrder() {
		inOrder(root);
	}

	/// <summary>
	/// BST方式插入，调整平衡
	/// </summary>
	/// <param name="value">插入的值</param>
	/// <returns>插入是否成功</returns>
	bool insert(const T& value) {
		node_t* pr = nil;
		node_t* s = root;
		while (s != nil) {
			if (value == s->key) {
				return false;
			}
			pr = s;
			if (value < s->key) {
				s = s->left;
			}
			else {
				s = s->right;
			}
		}

		s = buyNode(value);
		if (pr == nil) {
			root = s;
			root->parent = pr;
		}
		else {
			if (value < pr->key) {
				pr->left = s;
			}
			else {
				pr->right = s;
			}
			s->parent = pr;
		}
		insertWithFixUp(s);
		return true;
	}

	/// <summary>
	/// 删除key结点
	/// </summary>
	/// <param name="key">结点值</param>
	/// <returns>删除是否成功</returns>
	void remove(T key) {
		node_t* t;
		if ((t = search(root, key)) != nil) {
			remove(t);
		}
		else {
			std::cout << "key isn't exists!";
		}
	}

	void inorderPrint() {
		inorderPrint(root);
	}

protected:
	/// <summary>
	/// 生成新的结点
	/// </summary>
	/// <param name="x">key初值</param>
	/// <returns>指向node的指针</returns>
	node_t* buyNode(const T& x = T()) {
		node_t* s(new node_t());
		assert(s != nullptr && "内存申请失败");
		s->color = RED;
		s->left = s->right = s->parent = nil;
		s->key = x;

		return s;
	}

/// <summary>
/// 中序遍历
/// </summary>
/// <typeparam name="T">数据类型</typeparam>
	void inOrder(node_t* root) {
		if (root != nil) {
			inOrder(root->left);
			std::cout << root->key << ' ';
			inOrder(root->right);
		}
	}

	/// <summary>
	/// 结点左旋
	/// </summary>
	/// <param name="z">旋转的结点</param>
	void leftRotate(node_t* z) {
		node_t* y = z->right;
		z->right = y->left;
		if (y->left != nil) {
			y->left->parent = z;
		}
		y->parent = z->parent;
		if (root == z) {
			root = y;
		}
		else if (z == z->parent->left) {
			z->parent->left = y;
		}
		else {
			z->parent->right = y;
		}
		y->left = z;
		z->parent = y;
	}

	/// <summary>
	/// 结点右旋
	/// </summary>
	/// <param name="z">旋转的结点</param>
	void rightRotate(node_t* z) {
		node_t* y = z->left;
		z->left = y->right;
		if (y->right != nil) {
			y->right->parent = z;
		}
		y->parent = z->parent;
		if (root == z) {
			root = y;
		}
		else if (z == z->parent->right) {
			z->parent->right = y;
		}
		else {
			z->parent->left = y;
		}
		y->right = z;
		z->parent = y;
	}

	void insertWithFixUp(node_t* s) {
		node_t* uncle;
		while (s->parent->color == RED) {
			if (s->parent == s->parent->parent->left) {
				uncle = s->parent->parent->right;
				if (uncle->color == RED) {
					s->parent->color = BLACK;
					uncle->color = BLACK;
					s->parent->parent->color = RED;
					s = s->parent->parent;
				}
				else {
					if (s == s->parent->right) {
						s = s->parent;
						leftRotate(s);
					}
					s->parent->color = BLACK;
					s->parent->parent->color = RED;
					rightRotate(s->parent->parent);
				}
			}
			else {
				if (s->parent == s->parent->parent->right) {
					uncle = s->parent->parent->left;
					if (uncle->color == RED) {
						s->parent->color = BLACK;
						uncle->color = BLACK;
						s->parent->parent->color = RED;
						s = s->parent->parent;
					}
					else {
						if (s == s->parent->left) {
							s = s->parent;
							rightRotate(s);
						}
						s->parent->color = BLACK;
						s->parent->parent->color = RED;
						leftRotate(s->parent->parent);
					}
				}
			}
		}
		root->color = BLACK;
	}

	/// <summary>
	/// 查找特定结点
	/// </summary>
	/// <param name="root">开始查找的结点</param>
	/// <param name="key">查找对应的值</param>
	/// <returns>是否查找到</returns>
	node_t* search(node_t* root, T key) const {
		if (root == nil) {
			return nil;
		}

		if (root->key == key) {
			return root;
		}

		if (key < root->key) {
			return search(root->left, key);
		}
		else {
			return search(root->right, key);
		}
	}

	/// <summary>
	/// 将u的子节点指向改为v，v的父节点指向改为u的父节点
	/// </summary>
	/// <param name="u">u节点</param>
	/// <param name="v">v结点</param>
	void transplant(node_t* u, node_t* v) {
		if (u->parent == nil) {
			root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	/// <summary>
	/// 查找最小值
	/// </summary>
	/// <param name="x">查找始节点</param>
	/// <returns>最小节点</returns>
	node_t* minimum(node_t* x) {
		if (x->left == nil) {
			return x;
		}
		return minimum(x->left);
	}

	/// <summary>
	/// 删除红黑树节点
	/// </summary>
	/// <param name="z">要删除的节点</param>
	void remove(node_t* z) {
		node_t* x = nil;
		node_t* y = z;
		Color yColor = y->color;
		if (z->left == nil) {
			x = z->right;
			transplant(z, z->right);
		}
		else if (z->right == nil) {
			x = z->left;
			transplant(z, z->left);
		}
		else {
			y = minimum(z->right);
			yColor = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				transplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			transplant(z, y);
			y->left = z->left;
			z->left->parent = y;
			y->color = z->color;
		}
		if (yColor == BLACK) {
			removeWithFixUp(x);
		}
	}

	/// <summary>
	/// 红黑树删除调整
	/// </summary>
	/// <param name="x">删除的节点</param>
	void removeWithFixUp(node_t* x) {
		while (x != root && x->color == BLACK) {
			if (x == x->parent->left) {
				node_t* w = x->parent->right;
				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					leftRotate(x->parent);
					w = x->parent->right;
				}
				else if (w->left->color == BLACK && w->right->color == BLACK) {
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->right->color == BLACK) {
						w->color = RED;
						w->left->color = BLACK;
						rightRotate(w);
						w = x->parent->right;
					}
					w->color = w->parent->color;
					w->parent->color = BLACK;
					w->right->color = BLACK;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				node_t* w = x->parent->left;
				if (w->color == RED) {
					w->parent->color = RED;
					w->color = BLACK;
					rightRotate(x->parent);
					w = x->parent->left;
				}

				if (w->right->color == BLACK && w->left->color == BLACK) {
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->left->color == BLACK) {
						w->right->color = BLACK;
						w->color = RED;
						leftRotate(w);
						w = x->parent->left;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = BLACK;
	}

	/// <summary>
	/// 析构红黑树
	/// </summary>
	/// <param name="r">根结点</param>
	void destroy(node_t*& r) {
		if (r == nil) {
			return;
		}
		if (r->left != nil) {
			destroy(r->left);
		}
		if (r->right != nil) {
			destroy(r->right);
		}
		delete r;
		r = nullptr;
	}

	void inorderPrint(node_t* node) {
		if (node == nil) {
			return;
		}

		if (node->left != nil) {
			inorderPrint(node->left);
		}
		std::cout << node->key << "(" << ((node->color == BLACK) ? "BLACK" : "RED") << ")" << ' ';
		if (node->right != nil) {
			inorderPrint(node->right);
		}
	}

private:
	node_t* root{};
	node_t* nil{};
};

