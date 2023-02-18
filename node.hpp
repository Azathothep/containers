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

		node(T const & v) : color(BLACK), value(v), parent(NULL), right(NULL), left(NULL) { }
	};
}


#endif