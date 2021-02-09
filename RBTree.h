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
	/// �������
	/// </summary>
	void inOrder() {
		inOrder(root);
	}

	/// <summary>
	/// BST��ʽ���룬����ƽ��
	/// </summary>
	/// <param name="value">�����ֵ</param>
	/// <returns>�����Ƿ�ɹ�</returns>
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
	/// ɾ��key���
	/// </summary>
	/// <param name="key">���ֵ</param>
	/// <returns>ɾ���Ƿ�ɹ�</returns>
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
	/// �����µĽ��
	/// </summary>
	/// <param name="x">key��ֵ</param>
	/// <returns>ָ��node��ָ��</returns>
	node_t* buyNode(const T& x = T()) {
		node_t* s(new node_t());
		assert(s != nullptr && "�ڴ�����ʧ��");
		s->color = RED;
		s->left = s->right = s->parent = nil;
		s->key = x;

		return s;
	}

/// <summary>
/// �������
/// </summary>
/// <typeparam name="T">��������</typeparam>
	void inOrder(node_t* root) {
		if (root != nil) {
			inOrder(root->left);
			std::cout << root->key << ' ';
			inOrder(root->right);
		}
	}

	/// <summary>
	/// �������
	/// </summary>
	/// <param name="z">��ת�Ľ��</param>
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
	/// �������
	/// </summary>
	/// <param name="z">��ת�Ľ��</param>
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
	/// �����ض����
	/// </summary>
	/// <param name="root">��ʼ���ҵĽ��</param>
	/// <param name="key">���Ҷ�Ӧ��ֵ</param>
	/// <returns>�Ƿ���ҵ�</returns>
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
	/// ��u���ӽڵ�ָ���Ϊv��v�ĸ��ڵ�ָ���Ϊu�ĸ��ڵ�
	/// </summary>
	/// <param name="u">u�ڵ�</param>
	/// <param name="v">v���</param>
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
	/// ������Сֵ
	/// </summary>
	/// <param name="x">����ʼ�ڵ�</param>
	/// <returns>��С�ڵ�</returns>
	node_t* minimum(node_t* x) {
		if (x->left == nil) {
			return x;
		}
		return minimum(x->left);
	}

	/// <summary>
	/// ɾ��������ڵ�
	/// </summary>
	/// <param name="z">Ҫɾ���Ľڵ�</param>
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
	/// �����ɾ������
	/// </summary>
	/// <param name="x">ɾ���Ľڵ�</param>
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
	/// ���������
	/// </summary>
	/// <param name="r">�����</param>
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

