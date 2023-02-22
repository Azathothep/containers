#ifndef NODE_HPP
# define NODE_HPP

# define BLACK false
# define RED true

namespace ft {

	template <typename T>
	struct node {
		bool color;
		T value;
		struct node * parent;
		struct node * right;
		struct node * left;

		node(T const & v) : color(RED), value(v), parent(NULL), right(NULL), left(NULL) { }

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
	};
}


#endif