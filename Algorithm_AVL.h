#ifndef ALGORITHM_AVL_H_
#define ALGORITHM_AVL_H_
#include "Algorithm_BST.h"

using namespace std;
#define Balanced(x) (stature((x).lChild) == stature((x).rChild)) //理想平衡条件
#define BalFac(x) (stature((x).lChild) - stature((x).rChild)) //平衡因子
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2)) //AVL平衡条件
/******************************************************************************************
* 在左、右孩子中叏更高者，若等高，则不父亲p同侧者优先
* 在AVL平衡调整前，倚此确定重极斱案
******************************************************************************************/
#define tallerChild(x) (\
stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : ( \
stature((x)->rChild) > stature((x)->lChild) ? (x)->rChild : ( \
IsLChild(*(x)) ? (x)->lChild : (x)->rChild \
) \
) \
)
template <typename T> class AVL : public BST<T> 
{	//由BST派生AVL树模板类
	public:
		BinNodePosi(T) insert(const T& e); //插入（重写）
		bool remove(const T& e); //删除（重写）
		// BST::search()等其余接口可直接沿用
};

template <typename T> BinNodePosi(T) AVL<T>::insert(const T& e) 
{	//将关键码 e 插入AVL树中
	BinNodePosi(T) & x = search(e); if (x) return x; //确认目标节点不存在（留意对_hot的设置）
	x = new BinNode<T>(e, _hot); _size++; //创建节点x（此后，其父_hot可能增高，祖父可能失衡）
	for (BinNodePosi(T) g = _hot; g; g = g->parent) 
	{	//从x之父出发向上，逐局检查各代祖先g
		if (!AvlBalanced(*g)) 
		{	//一旦发现g失衡，则（采用“ 3 + 4”算法）使之复衡
			FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //将该子树联至原父亲
			break; //g复衡后，局部子树高度必然复原；其祖先亦必如此，故调整随即结束
		}
		else //否则（g依然平衡），叧需简单地
			updateHeight(g); //更新其高度（注意：即便g未失衡，高度亦可能增加）
	} //至于叧需一次调整；若果真做过调整，则全树高度必然复原
	return x; //迒回新节点
} //无论是否存在亍原树中，返回时总有x->data == e

template <typename T> bool AVL<T>::remove(const T& e) 
{	//从AVL树中删除关键码e
	BinNodePosi(T) & x = search(e); if (!x) return false; //确认目标节点存在（留意对_hot的设置）
	removeAt(x, _hot); _size--; //先按BST规则删除之（此后，原节点之父_hot及其祖先均可能失衡）
	for (BinNodePosi(T) g = _hot; g; g = g->parent) 
	{	//从_hot出发向上，逐步检查各代祖先g
		if (!AvlBalanced(*g)) //一旦发现g失衡，则（采用“ 3 + 4”算法）使之复衡
			g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //将该子树联至原父亲
		updateHeight(g); //幵更新其高度（注意：即便g未失衡，高度亦可能降低）
	} //可能需做 Omega(logn) 次调整——无论是否做过调整，全树高度均可能降低
	return true; //删除成功
} //若目标节点存在且被删除，返回true；否则返回false

/******************************************************************************************
  * 按照“ 3 + 4”构造联接3个节点及其四棵子树，返回重组之后癿局部子树根节点位置（即b）
  * 子树根节点与上层节点之间的双向联接，均项由上局调用者完成
  * 可用与AVL和RedBlack癿局部平衡调整
******************************************************************************************/
template <typename T> BinNodePosi(T) BST<T>::connect34(
	BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
	BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3
	) {
	a->lChild = T0; if (T0) T0->parent = a;
	a->rChild = T1; if (T1) T1->parent = a; updateHeight(a);
	c->lChild = T2; if (T2) T2->parent = c;
	c->rChild = T3; if (T3) T3->parent = c; updateHeight(c);
	b->lChild = a; a->parent = b;
	b->rChild = c; c->parent = b; updateHeight(b);
	return b; //返子树新的根节点
}

/******************************************************************************************
* BST节点旋转变换统一算法（3节点 + 4子树），返回调整之后布局子树根节点的位置
* 注意：尽管子树根会正确指向上局节点（如果存在），但反向的联接项由上局函数完成
******************************************************************************************/
template <typename T> BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) 
{	//v为非空的孙辈节点
	BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent; //规v、 p和g相对位置分四种情况
	if (IsLChild(*p)) /* zig */
		if (IsLChild(*v)) { /* zig-zig */
		p->parent = g->parent; //向上联接
		return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
	}
	else { /* zig-zag */
		v->parent = g->parent; //向上联接
		return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
	}
	else /* zag */
		if (IsRChild(*v)) { /* zag-zag */
		p->parent = g->parent; //向上联接
		return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
	}
	else { /* zag-zig */
			v->parent = g->parent; //向上联接
			return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
	}
}
#endif
