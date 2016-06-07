#ifndef ALGORITHM_BST_H_
#define ALGORITHM_BST_H_

#include "Algorithm_Binary_Tree.h"
using namespace std; 

template <typename T> class BST : public BinTree<T> 
{	//��BinTree����BSTģ����
	protected:
		BinNodePosi(T) _hot; //BST::search()�����ʵķǿգ��������գ��Ľڵ�λ��
		BinNodePosi(T) connect34( //���ա� 3 + 4���ṹ������3���ڵ㼰�Ŀ�����
			BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
			BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
		BinNodePosi(T) rotateAt(BinNodePosi(T) x); //��x���丸�ס��游��ͳһ��ת����
	public:
		//�����ӿڣ���virtual���Σ�ǿ��Ҫ�����������ࣨBST���֣����ݸ��ԵĹ��������д
		virtual BinNodePosi(T) & search(const T& e); //����
		virtual BinNodePosi(T) insert(const T& e); //����
		virtual bool remove(const T& e); //ɾ��
};

template < typename T> //����vΪ���ģ�AVL�� SPLAY�� rbTree�ȣ�BST�����в��ҹؼ��� e
static BinNodePosi(T) & searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot) 
{
	if (!v || (e == v->data)) return v; //�ݹ�����ڽڵ�v��������ͨ��ڵ㣩������
	hot = v; //һ��������ȼ��µ�ǰ�ڵ㣬Ȼ����
	return searchIn(((e < v->data) ? v->lChild : v->rChild), e, hot); //����һ�㣬�ݹ����
} //����ʱ������ֵָ�����нڵ㣨������ͨ���ڱ�����hotָ���丸�ף��˻�ʱΪ��ʼֵNULL��

template <typename T> BinNodePosi(T) & BST<T>::search(const T& e) //��BST�в��ҹؼ���e
{ return searchIn(_root, e, _hot = NULL); } //����Ŀ��ڵ�λ�õ����ã��Ա�������롢ɾ������

template <typename T> BinNodePosi(T) BST<T>::insert(const T& e) 
{	//���ؼ��� e ����BST����
	BinNodePosi(T) & x = search(e); if (x) return x; //ȷ��Ŀ��ڵ㲻���ڣ������_hot�����ã�
	x = new BinNode<T>(e, _hot); //�����½ڵ�x����eΪ�ؼ��룬��_hotΪ��
	_size++; //����ȫ����ģ
	updateHeightAbove(x); //����x�����������ȵĸ߶�
	return x; //�²���Ľڵ㣬��ΪҶ��
} //���� e �Ƿ������ԭ���У�����ʱ���� x->data == e

template <typename T> bool BST<T>::remove(const T& e) 
{	//��BST����ɾ���ؼ���e
	BinNodePosi(T) & x = search(e); if (!x) return false; //ȷ��Ŀ��ڵ���ڣ������_hot �����ã�
	removeAt(x, _hot); _size--; //ʵʩɾ��
	updateHeightAbove(_hot); //����_hot�����������ȵĸ߶�
	return true;
} //ɾ���ɹ������ɷ���ֵָʾ

  /******************************************************************************************
  * BST�ڵ�ɾ���㷨��ɾ��λ��x��ָ�Ľڵ㣨ȫ�־�̬ģ�庯����������AVL�� Splay�� RedBlack�ȸ���BST��
  * x�ɴ�ǰ�Ĳ���ȷ��������ȷ�Ϸ�NULL��ڵ��ñ��������ʱ�ɾ���ɹ�
  * ��searchIn��ͬ������ǰ���ؽ�hot�ÿ�
  * ����ֵָ��ʵ�ʱ�ɾ���ڵ�Ľ����ߣ�hotָ��ʵ�ʱ�ɾ���ڵ�ĸ��ס������߾��п�����NULL
  ******************************************************************************************/
template <typename T> static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
	BinNodePosi(T) w = x; //ʵ�ʱ�ժ���Ľڵ㣬��ֵͬx
	BinNodePosi(T) succ = NULL; //ʵ�ʱ�ɾ���ڵ�Ľ�����
	if (!HasLChild(*x)) //��*x��������Ϊ�գ����
		succ = x = x->rChild; //ֱ�ӽ�*x�滻Ϊ��������
	else if (!HasRChild(*x)) //��������Ϊ�գ����
		succ = x = x->lChild; //�ԳƵش�����ע�⣺��ʱsucc != NULL
	else { //���������������ڣ���ѡ��x��ֱ�Ӻ����Ϊʵ�ʱ�ժ���ڵ㣬Ϊ����Ҫ
		w = w->succ(); //�����������У��ҵ�*x��ֱ�Ӻ��*w
		swap(x->data, w->data); //����*x��*w������Ԫ��
		BinNodePosi(T) u = w->parent;
		((u == x) ? u->rChild : u->lChild) = succ = w->rChild; //����ڵ�*w
	}
	hot = w->parent; //��¼ʵ�ʱ�ɾ���ڵ�ĸ���
	if (succ) succ->parent = hot; //����ɾ���ڵ�Ľ�������hot����
	release(w->data); release(w); return succ; //�ͷű�ժ���ڵ㣬���ؽ�����
}
#endif
