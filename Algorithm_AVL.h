#ifndef ALGORITHM_AVL_H_
#define ALGORITHM_AVL_H_
#include "Algorithm_BST.h"

using namespace std;
#define Balanced(x) (stature((x).lChild) == stature((x).rChild)) //����ƽ������
#define BalFac(x) (stature((x).lChild) - stature((x).rChild)) //ƽ������
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2)) //AVLƽ������
/******************************************************************************************
* �����Һ����Ѕ������ߣ����ȸߣ��򲻸���pͬ��������
* ��AVLƽ�����ǰ���д�ȷ���ؼ��ڰ�
******************************************************************************************/
#define tallerChild(x) (\
stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : ( \
stature((x)->rChild) > stature((x)->lChild) ? (x)->rChild : ( \
IsLChild(*(x)) ? (x)->lChild : (x)->rChild \
) \
) \
)
template <typename T> class AVL : public BST<T> 
{	//��BST����AVL��ģ����
	public:
		BinNodePosi(T) insert(const T& e); //���루��д��
		bool remove(const T& e); //ɾ������д��
		// BST::search()������ӿڿ�ֱ������
};

template <typename T> BinNodePosi(T) AVL<T>::insert(const T& e) 
{	//���ؼ��� e ����AVL����
	BinNodePosi(T) & x = search(e); if (x) return x; //ȷ��Ŀ��ڵ㲻���ڣ������_hot�����ã�
	x = new BinNode<T>(e, _hot); _size++; //�����ڵ�x���˺��丸_hot�������ߣ��游����ʧ�⣩
	for (BinNodePosi(T) g = _hot; g; g = g->parent) 
	{	//��x֮���������ϣ���ּ���������g
		if (!AvlBalanced(*g)) 
		{	//һ������gʧ�⣬�򣨲��á� 3 + 4���㷨��ʹ֮����
			FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //������������ԭ����
			break; //g����󣬾ֲ������߶ȱ�Ȼ��ԭ�������������ˣ��ʵ����漴����
		}
		else //����g��Ȼƽ�⣩������򵥵�
			updateHeight(g); //������߶ȣ�ע�⣺����gδʧ�⣬�߶���������ӣ�
	} //���څ���һ�ε�����������������������ȫ���߶ȱ�Ȼ��ԭ
	return x; //ކ���½ڵ�
} //�����Ƿ����ءԭ���У�����ʱ����x->data == e

template <typename T> bool AVL<T>::remove(const T& e) 
{	//��AVL����ɾ���ؼ���e
	BinNodePosi(T) & x = search(e); if (!x) return false; //ȷ��Ŀ��ڵ���ڣ������_hot�����ã�
	removeAt(x, _hot); _size--; //�Ȱ�BST����ɾ��֮���˺�ԭ�ڵ�֮��_hot�������Ⱦ�����ʧ�⣩
	for (BinNodePosi(T) g = _hot; g; g = g->parent) 
	{	//��_hot�������ϣ��𲽼���������g
		if (!AvlBalanced(*g)) //һ������gʧ�⣬�򣨲��á� 3 + 4���㷨��ʹ֮����
			g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //������������ԭ����
		updateHeight(g); //�Ը�����߶ȣ�ע�⣺����gδʧ�⣬�߶�����ܽ��ͣ�
	} //�������� Omega(logn) �ε������������Ƿ�����������ȫ���߶Ⱦ����ܽ���
	return true; //ɾ���ɹ�
} //��Ŀ��ڵ�����ұ�ɾ��������true�����򷵻�false

/******************************************************************************************
  * ���ա� 3 + 4����������3���ڵ㼰���Ŀ���������������֮��m�ֲ��������ڵ�λ�ã���b��
  * �������ڵ����ϲ�ڵ�֮���˫�����ӣ��������Ͼֵ��������
  * ������AVL��RedBlack�m�ֲ�ƽ�����
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
	return b; //�������µĸ��ڵ�
}

/******************************************************************************************
* BST�ڵ���ת�任ͳһ�㷨��3�ڵ� + 4�����������ص���֮�󲼾��������ڵ��λ��
* ע�⣺��������������ȷָ���Ͼֽڵ㣨������ڣ�������������������Ͼֺ������
******************************************************************************************/
template <typename T> BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) 
{	//vΪ�ǿյ��ﱲ�ڵ�
	BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent; //��v�� p��g���λ�÷��������
	if (IsLChild(*p)) /* zig */
		if (IsLChild(*v)) { /* zig-zig */
		p->parent = g->parent; //��������
		return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
	}
	else { /* zig-zag */
		v->parent = g->parent; //��������
		return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
	}
	else /* zag */
		if (IsRChild(*v)) { /* zag-zag */
		p->parent = g->parent; //��������
		return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
	}
	else { /* zag-zig */
			v->parent = g->parent; //��������
			return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
	}
}
#endif
