#ifndef NODE_HPP
# define NODE_HPP

# define BLACK_NODE 0
# define RED_NODE 1

namespace ft {

	template <typename T>
	struct node {
		char color;
		T value;
		struct node *parent;
		struct node *right;
		struct node *left;

		node() : color(BLACK_NODE), parent(NULL), right(NULL), left(NULL) { }

		node(T const & v) : color(RED_NODE), value(v), parent(NULL), right(NULL), left(NULL) { }

		node *grandparent() {
			if (this->parent)
				return this->parent->parent;
			return NULL;
		}

		node *aunt() {
			if (this->grandparent() == NULL)
					return NULL;

			if (this->parent == this->grandparent()->left)
				return this->grandparent()->right;
			else
				return this->grandparent()->left;
		}

		node *sibling() {
			if (this->parent == NULL)
				return NULL;

			if (this == this->parent->left)
				return this->parent->right;
			return this->parent->left;
		}
	};
}


#endif