#ifndef ALGORITHM_BST_H_
#define ALGORITHM_BST_H_

#include "Algorithm_Binary_Tree.h"
using namespace std; 

template <typename T> class BST : public BinTree<T> 
{	//由BinTree派生BST模板类
	protected:
		BinNodePosi(T) _hot; //BST::search()最后访问的非空（除非树空）的节点位置
		BinNodePosi(T) connect34( //按照“ 3 + 4”结构，联接3个节点及四棵子树
			BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
			BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
		BinNodePosi(T) rotateAt(BinNodePosi(T) x); //对x及其父亲、祖父做统一旋转调整
	public:
		//基本接口：以virtual修饰，强制要求所有派生类（BST变种）根据各自的规则对其重写
		virtual BinNodePosi(T) & search(const T& e); //查找
		virtual BinNodePosi(T) insert(const T& e); //插入
		virtual bool remove(const T& e); //删除
};

template < typename T> //在以v为根的（AVL、 SPLAY、 rbTree等）BST子树中查找关键码 e
static BinNodePosi(T) & searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot) 
{
	if (!v || (e == v->data)) return v; //递归基：在节点v（或假想的通配节点）处命中
	hot = v; //一般情况：先记下当前节点，然后再
	return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot); //深入一层，递归查找
} //返回时，返回值指向命中节点（或假想的通配哨兵），hot指向其父亲（退化时为初始值NULL）

template <typename T> BinNodePosi(T) & BST<T>::search(const T& e) //在BST中查找关键码e
{ return searchIn(_root, e, _hot = NULL); } //返回目标节点位置的引用，以便后续插入、删除操作

template <typename T> BinNodePosi(T) BST<T>::insert(const T& e) 
{	//将关键码 e 插入BST树中
	BinNodePosi(T) & x = search(e); if (x) return x; //确认目标节点不存在（留意对_hot的设置）
	x = new BinNode<T>(e, _hot); //创建新节点x：以e为关键码，以_hot为父
	_size++; //更新全树规模
	updateHeightAbove(x); //更新x及其历代祖先的高度
	return x; //新插入的节点，必为叶子
} //无论 e 是否存在于原树中，返回时总有 x->data == e

template <typename T> bool BST<T>::remove(const T& e) 
{	//从BST树中删除关键码e
	BinNodePosi(T) & x = search(e); if (!x) return false; //确认目标节点存在（留意对_hot 的设置）
	removeAt(x, _hot); _size--; //实施删除
	updateHeightAbove(_hot); //更新_hot及其历代祖先的高度
	return true;
} //删除成功不否，由返回值指示

  /******************************************************************************************
  * BST节点删除算法：删除位置x所指的节点（全局静态模板函数，适用于AVL、 Splay、 RedBlack等各种BST）
  * x由此前的查找确定，经过确认非NULL后斱调用本函数，故必删除成功
  * 与searchIn不同，调用前不必将hot置空
  * 返回值指向实际被删除节点的接替者，hot指向实际被删除节点的父亲——二者均有可能是NULL
  ******************************************************************************************/
template <typename T> static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
	BinNodePosi(T) w = x; //实际被摘除的节点，初值同x
	BinNodePosi(T) succ = NULL; //实际被删除节点的接替者
	if (!HasLChild(*x)) //若*x的左子树为空，则可
		succ = x = x->rChild; //直接将*x替换为其右子树
	else if (!HasRChild(*x)) //若右子树为空，则可
		succ = x = x->lChild; //对称地处理——注意：此时succ != NULL
	else { //若左右子树均存在，则选择x的直接后继作为实际被摘除节点，为此需要
		w = w->succ(); //（在右子树中）找到*x的直接后继*w
		swap(x->data, w->data); //交换*x和*w的数据元素
		BinNodePosi(T) u = w->parent;
		((u == x) ? u->rChild : u->lChild) = succ = w->rChild; //隔离节点*w
	}
	hot = w->parent; //记录实际被删除节点的父亲
	if (succ) succ->parent = hot; //将被删除节点的接替者与hot相联
	release(w->data); release(w); return succ; //释放被摘除节点，返回接替者
}
#endif
