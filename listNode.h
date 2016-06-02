typedef int Rank; //��
#define ListNodePosi(T) ListNode<T>* //�б�ڵ�λ��

template <typename T> struct ListNode
{	//�б�ڵ�ģ���ࣨ��˫��������ʽʵ�֣�
	// ��Ա
	T data; //��ֵ
	ListNodePosi(T) pred; //ǰ��
	ListNodePosi(T) succ; //���
	// ���캯��
	ListNode() {} //���header��trailer�m����
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL) : data(e), pred(p), succ(s) {} //Ĭ�Ϲ��캯��
	// �����ӿ�
	ListNodePosi(T) insertAsPred(T const& e); //������ǰ�ڵ�֮ǰ�����½ڵ�
	ListNodePosi(T) insertAsSucc(T const& e); //���浱ǰ�ڵ�֮������½ڵ�
}; 
