#ifndef ALGORITHM_BINARY_TREE_H_
#define ALGORITHM_BINARY_TREE_H_
using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		�����Ƕ�������������Լ�����
//
//
#define BinNodePosi(T) BinNode<T>* //�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1) //�ڵ�߶ȣ��롰�����߶�Ϊ-1����Լ����ͳһ��
typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ

template <typename T> struct BinNode { //�������ڵ�ģ����
	// ��Ա��Ϊ���������ͳһ���ţ����߿ɸ�����Ҫ��һ����װ��
	T data; //��ֵ
	BinNodePosi(T) parent; BinNodePosi(T) lChild; BinNodePosi(T) rChild; //���ڵ㼰���Һ���
	int height; //�߶ȣ�ͨ�ã�
	int npl; //Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩
	RBColor color; //��ɫ���������
	// ���캯��
	BinNode() : parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED) { }
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
			int h = 0, int l = 1, RBColor c = RB_RED): data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) { }
	// �����ӿ�
	int size(); //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ
	BinNodePosi(T) insertAsLC(T const&); //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
	BinNodePosi(T) insertAsRC(T const&); //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
	BinNodePosi(T) succ(); //ȡ��ǰ�ڵ��ֱ�Ӻ��
	template <typename VST> void travLevel(VST&); //�����ֲ�����
	template <typename VST> void travPre(VST&); //�����������
	template <typename VST> void travIn(VST&); //�����������
	template <typename VST> void travPost(VST&); //�����������
	// �Ƚ������е�����������һ���������в��䣩
	bool operator<(BinNode const& bn) { return data < bn.data; } //С��
	bool operator==(BinNode const& bn) { return data == bn.data; } //����
};


/******************************************************************************************
 * BinNode״̬�����ʵ��ж�
******************************************************************************************/
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lChild))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //����ӵ��һ������
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //ͬʱӵ����������
#define IsLeaf(x) (!HasChild(x))
/******************************************************************************************
 * ��BinNode�����ض���ϵ�Ľڵ㼰ָ��
******************************************************************************************/
#define sibling(p) (\IsLChild(*(p)) ? \(p)->parent->rChild : \(p)->parent->lChild \) //�ֵ�
#define uncle(x) (\IsLChild(*((x)->parent)) ? \(x)->parent->parent->rChild : \(x)->parent->parent->lChild \) //����
#define FromParentTo(x) (\IsRoot(x) ? _root : ( \IsLChild(x) ? (x).parent->lChild : (x).parent->rChild \) \) //���Ը��׵�ָ��

template <typename T> //��e��Ϊ��ǰ�ڵ�����Ӳ��������
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) { return lChild = new BinNode(e, this); }

template <typename T> //��e��Ϊ��ǰ�ڵ���Һ��Ӳ��������
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) { return rChild = new BinNode(e, this); }

template <typename T> template <typename VST> //Ԫ�����͡�������
void BinNode<T>::travIn(VST& visit) 
{	//��������������㷨ͳһ���
	switch (rand() % 5) { //�˴������ѡ���������ԣ�������ѡ��
		case 1: travIn_I1(this, visit); break; //������#1
		case 2: travIn_I2(this, visit); break; //������#2
		case 3: travIn_I3(this, visit); break; //������#3
		default: travIn_R(this, visit); break; //���ư�
	}
}
//
//
//		��������������Զ������
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> class BinTree 
{	//������ģ����
	protected:
		int _size; //��ģ
		BinNodePosi(T) _root; //���ڵ�
		virtual int updateHeight(BinNodePosi(T) x); //���½ڵ�x�m�߶�
		void updateHeightAbove(BinNodePosi(T) x); //���½ڵ�x�������Ȱm�߶�
	public:
		BinTree() : _size(0), _root(NULL) { } //���캯��
		~BinTree() { if (0 < _size) remove(_root); } //��������
		int size() const { return _size; } //��ģ
		bool empty() const { return !_root; } //�п�
		BinNodePosi(T) root() const { return _root; } //����
		BinNodePosi(T) insertAsRoot(T const& e); //������ڵ�
		BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e��Ϊx�����ӣ�ԭ�ޣ�����
		BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e��Ϊx���Һ��ӣ�ԭ�ޣ�����
		BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T); //T��Ϊx����������
		BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T); //T��Ϊx����������
		int remove(BinNodePosi(T) x); //������λ��x���ڵ�Ϊ��������������������ԭ�ȵĹ�ģ
		BinTree<T>* secede(BinNodePosi(T) x); //������x�ӵ�ǰ����ժ����������ת��Ϊһ�ö�������
		template < typename VST> //������
		void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } //�ֲ�����
		template < typename VST> //������
		void travPre(VST& visit) { if (_root) _root->travPre(visit); } //�������
		template < typename VST> //������
		void travIn(VST& visit) { if (_root) _root->travIn(visit); } //�������
		template < typename VST> //������
		void travPost(VST& visit) { if (_root) _root->travPost(visit); } //�������
		// �Ƚ������е�����������һ���������в��䣩
		bool operator<(BinTree<T> const& t) { return _root && t._root && lt(_root, t._root); }
		bool operator==(BinTree<T> const& t) { return _root && t._root && (_root == t._root); }
}; //BinTree

template <typename T> int BinTree<T>::updateHeight(BinNodePosi(T) x) //���½ڵ�x�߶�
{	
	return x->height = 1 + max(stature(x->lChild), stature(x->rChild)); 
} //�����ģ������ͬ����

template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //����x�����ȵĸ߶�
{	while (x)
    {
      updateHeight(x);    //���Ż���һ���߶�δ�䣬������ֹ
       x = x->parent;
	 } 
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{ _size = 1; return _root = new BinNode<T>(e); } //��e�������ڵ����յĶ�����

template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{ _size++; x->insertAsLC(e); updateHeightAbove(x); return x->lChild; } //e����Ϊx������

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{ _size++; x->insertAsRC(e); updateHeightAbove(x); return x->rChild; } //e����Ϊx���Һ���


template <typename T> //���������������㷨����S�����ڵ�x�����������룬S�����ÿ�
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S) 
{	//x->lChild == NULL
	if (x->lChild = S->_root) x->lChild->parent = x; //����
	_size += S->_size; updateHeightAbove(x); //����ȫ����ģ��x�������ȵĸ߶�
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x; //�ͷ�ԭ�������ؽ���λ��
}

template <typename T> //���������������㷨����S�����ڵ�x�����������룬S�����ÿ�
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S) 
{	//x->rChild == NULL
	if (x->rChild = S->_root) x->rChild->parent = x; //����
	_size += S->_size; updateHeightAbove(x); //����ȫ����ģ��x�������ȵĸ߶�
	S->_root = NULL; S->_size = 0; release(S); S = NULL; return x; //�ͷ�ԭ�������ؽ���λ��
}

template <typename T> //ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
int BinTree<T>::remove(BinNodePosi(T) x) 
{	//assert: xΪ�������еĺϷ�λ��
	FromParentTo(*x) = NULL; //�ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent); //�������ȸ߶�
	int n = removeAt(x); _size -= n; return n; //ɾ������x�����¹�ģ������ɾ���ڵ�����
}

template <typename T> //ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
static int removeAt(BinNodePosi(T) x) 
{	//assert: xΪ�������еĺϷ�λ��
	if (!x) return 0; //�ݹ��������
	int n = 1 + removeAt(x->lChild) + removeAt(x->rChild); //�ݹ��ͷ���������
	release(x->data); release(x); return n; //�ͷű�ժ���ڵ㣬������ɾ���ڵ�����
}

template < typename T> //���������������㷨��������x�ӵ�ǰ����ժ���������װΪһ�ö�����������
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) 
{	//assert: xΪ�������еĺϷ�λ��
	FromParentTo(*x) = NULL; //�ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent); //����ԭ�����������ȵĸ߶�
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL; //������xΪ��
	S->_size = x->size(); _size -= S->_size; return S; //���¹�ģ�����ط������������
}

template <typename T, typename VST> //Ԫ�����͡�������
void travPre_R(BinNodePosi(T) x, VST& visit) { //��������������㷨���ݹ�棩
	if (!x) return;
	visit(x->data);
	travPre_R(x->lChild, visit);
	travPre_R(x->rChild, visit);
}

template < typename T, typename VST> //Ԫ�����͡�������
void travPost_R(BinNodePosi(T) x, VST& visit) { //��������������㷨���ݹ�棩
	if (!x) return;
	travPost_R(x->lChild, visit);
	travPost_R(x->rChild, visit);
	visit(x->data);
}

template < typename T, typename VST> //Ԫ�����͡�������
void travIn_R(BinNodePosi(T) x, VST& visit)   //��������������㷨���ݹ�棩
{
	if (!x) return;
	travIn_R(x->lChild, visit);
	visit(x->data);
	travIn_R(x->rChild, visit);
}

//�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ㣻�صݽڵ���������������
template <typename T, typename VST> //Ԫ�����͡�������
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S) 
{
	while (x) 
	{
		visit(x->data); //���ʵ�ǰ�ڵ�
		S.push(x->rChild); //�Һ�����ջ�ݴ棨���Ż���ͨ���жϣ�����յ��Һ�����ջ��
		x = x->lChild; //�����֧����һ��
	}
}

template <typename T, typename VST> //Ԫ�����͡�������
void travPre_I2(BinNodePosi(T) x, VST& visit) 
{	//��������������㷨��������#2��
	Stack<BinNodePosi(T)> S; //����ջ
	while (true) {
		visitAlongLeftBranch(x, visit, S); //�ӵ�ǰ�ڵ��������������
		if (S.empty()) break; //ֱ��ջ��
		x = S.pop(); //������һ���m���
	}
}

template <typename T> //�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ�
static void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S) 
{
	while (x) { S.push(x); x = x->lChild; } //��ǰ�ڵ���ջ���漴������֧���룬����ֱ��������
}

template <typename T, typename VST> //Ԫ�����͡�������
void travIn_I1(BinNodePosi(T) x, VST& visit) 
{	//��������������㷨��������#1��
	Stack<BinNodePosi(T)> S; //����ջ
	while (true) {
		goAlongLeftBranch(x, S); //�ӵ�ǰ�ڵ������������ջ
		if (S.empty()) break; //ֱ�����нڵ㴦�����
		x = S.pop(); visit(x->data); //����ջ���ڵ㲢����
		x = x->rChild; //ת��������
	}
}

template <typename T> BinNodePosi(T) BinNode<T>::succ() { //��λ�ڵ� v ��ֱ�Ӻ��
	BinNodePosi(T) s = this; //��¼��̵���ʱ����
	if (rChild) 
	{	//�����Һ��ӣ���ֱ�Ӻ�̱����������У�����ؾ���
		s = rChild; //��������
		while (HasLChild(*s)) s = s->lChild; //�����С���Ľڵ�
	}
	else 
	{	//����ֱ�Ӻ��Ӧ�ǡ�����ǰ�ڵ���������������е�������ȡ�������ؾ���
		while (IsRChild(*s)) s = s->parent; //������������֧�����ϳ����Ϸ��ƶ�
		s = s->parent; //����ٳ����Ϸ��ƶ�һ�������ִ�ֱ�Ӻ�̣�������ڣ�
	}
	return s;
}

template <typename T, typename VST> //Ԫ�����͡�������
void travIn_I2(BinNodePosi(T) x, VST& visit) 
{	//��������������㷨��������#2��
	Stack<BinNodePosi(T)> S; //����ջ
	while (true)
		if (x) 
		{
		S.push(x); //���ڵ��ջ
		x = x->lChild; //�������������
		}
		else if (!S.empty()) 
		{
			x = S.pop(); //��δ���ʵ�������Ƚڵ���ջ
			visit(x->data); //���ʸ����Ƚڵ�
			x = x->rChild; //�������ȵ�������
		}
		else
			break; //�������
}

template <typename T, typename VST> //Ԫ�����͡�������
void travIn_I3(BinNodePosi(T) x, VST& visit) 
{	//��������������㷨��������#3�����踨��ջ��
	bool backtrack = false; //ǰһ���Ƿ�մ����������ݡ���ʡȥջ����O(1)�����ռ�
	while (true)
		if (!backtrack && HasLChild(*x)) //�����������Ҳ��Ǹոջ��ݣ���
			x = x->lChild; //�������������
		else 
		{	//��������������ոջ��ݣ��൱������������
			visit(x->data); //���ʸýڵ�
			if (HasRChild(*x)) 
			{	//�����������ǿգ���
				x = x->rChild; //������������������
				backtrack = false; //���رջ��ݱ�־
			}
			else 
			{	//���������գ���
				if (!(x = x->succ())) break; //���ݣ����ִ�ĩ�ڵ�ʱ���˳����أ�
				backtrack = true; //�û��ݱ�־
			}
		}
}

template <typename T> //����Sջ���ڵ�Ϊ���������У��ҵ�������ɼ�Ҷ�ڵ�
static void gotoHLVFL(Stack<BinNodePosi(T)>& S) 
{	//��;�����ڵ�������ջ
	while (BinNodePosi(T) x = S.top()) //�Զ����£�������鵱ǰ�ڵ㣨��ջ����
		if (HasLChild(*x)) 
		{	//����������
			if (HasRChild(*x)) S.push(x->rChild); //�����Һ��ӣ�������ջ
			S.push(x->lChild); //Ȼ���ת������
		}
		else S.push(x->rChild); //ʵ�����Ѳ�����
	S.pop(); //����֮ǰ������ջ���Ŀսڵ�
}

template <typename T, typename VST>
void travPost_I(BinNodePosi(T) x, VST& visit) 
{	//�������ĺ�������������棩
	Stack<BinNodePosi(T)> S; //����ջ
	if (x) S.push(x); //���ڵ���ջ
	while (!S.empty()) 
	{
		if (S.top() != x->parent) //��ջ���ǵ�ǰ�ڵ�֮�������Ϊ�����֣�����ʱ��
			gotoHLVFL(S); //����������Ϊ��֮�����У��ҵ�HLVFL���൱�ڵݹ��������У�
		x = S.pop(); visit(x->data); //����ջ������ǰһ�ڵ�֮��̣���������
	}
}

template <typename T> template < typename VST> //Ԫ�����͡�������
void BinNode<T>::travLevel(VST& visit) 
{	//��������α����㷨
	Queue<BinNodePosi(T)> Q; //��������
	Q.enqueue(this); //���ڵ����
	while (!Q.empty()) { //�ڶ����ٴη���֮ǰ����������
		BinNodePosi(T) x = Q.dequeue(); visit(x->data); //ȡ�����׽ڵ㲢����
		if (HasLChild(*x)) Q.enqueue(x->lChild); //�������
		if (HasRChild(*x)) Q.enqueue(x->rChild); //�Һ������
	}
}
#endif
