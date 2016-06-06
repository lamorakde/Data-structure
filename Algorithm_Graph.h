#ifndef ALGORITHM_GRAPH_H_
#define ALGORITHM_GRAPH_H_
#include "Algorithm_Vector.h" //��������

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //����״̬
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus; //��״̬

using namespace std;
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//
//										ͼģ����
//
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
template <typename Tv,typename Te> //�������͡�������
class Graph 
{	//ͼGraphģ����
	private:
		void reset() 
		{	//���ж��㡢�ߵĸ�����Ϣ��λ
			for (int i = 0; i < n; i++) 
			{	
				status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1; //���ж����״̬��ʱ���ǩ
				parent(i) = -1; priority(i) = INT_MAX; //���ڱ������еģ����ڵ㣬���ȼ���
				for (int j = 0; j < n; j++) 
					if (exists(i, j)) status(i, j) = UNDETERMINED; //���б߰m״̬
			}
		}
		void BFS(int, int&); //����ͨ�򣩹�����������㷨
		void DFS(int, int&); //����ͨ��������������㷨
		void BCC(int, int&, Stack<int>&); //����ͨ�򣩻���DFS�m˫��ͨ�����ֽ��㷨
		bool TSort(int, int&, Stack<Tv>*); //����ͨ�򣩻���DFS�����������㷨
		template <typename PU> void PFS(int, PU); //����ͨ�����ȼ��������
	public:
		// ����
		int n; //��������
		virtual int insert(Tv const&) = 0; //���붥�㣬���ر��
		virtual Tv remove(int) = 0; //ɾ�����㼰������ߣ������䶥����Ϣ
		virtual Tv& vertex(int) = 0; //����v�����ݣ��ö���mȷ���ڣ�
		virtual int inDegree(int) = 0; //����v����ȣ��ö���mȷ���ڣ�
		virtual int outDegree(int) = 0; //����v�ĳ��ȣ��ö���mȷ���ڣ�
		virtual int firstNbr(int) = 0; //����v���׸��ڽӶ���
		virtual int nextNbr(int, int) = 0; //����v�ģ�����ڶ���j�ģ���һ�ڽӶ���
		virtual VStatus& status(int) = 0; //����v��״̬
		virtual int& dTime(int) = 0; //����v��ʱ���ǩdTime
		virtual int& fTime(int) = 0; //����v��ʱ���ǩfTime
		virtual int& parent(int) = 0; //����v�ڱ������еĸ���
		virtual int& priority(int) = 0; //����v�ڱ������е����ȼ���
		// �ߣ�����Լ��������߾�ͳһת��Ϊ������mһ������ߣ��Ӷ�������ͼ��������ͼ�m����
		int e; //������
		virtual bool exists(int, int) = 0; //��(v, u)�Ƿ����
		virtual void insert(Te const&, int, int, int) = 0; //�ڶ���v��u֮�����Ȩ��Ϊw�ı�e
		virtual Te remove(int, int) = 0; //ɾ������v��u֮��ı�e�����ظñ���Ϣ
		virtual EStatus& status(int, int) = 0; //��(v, u)��״̬
		virtual Te& edge(int, int) = 0; //��(v, u)�����ݣ��ñߵ�ȷ���ڣ�
		virtual int& weight(int, int) = 0; //��(v, u)��Ȩ��
		// �㷨
		void bfs(int); //������������㷨
		void dfs(int); //������������㷨
		void bcc(int); //��ءDFS��˫��ͨ�����ֽ��㷨
		Stack<Tv>* tSort(int); //��ءDFS�����������㷨
		void prim(int); //��С֧����Prim�㷨
		void dijkstra(int); //���·��Dijkstra�㷨
		template <typename PU> void pfs(int, PU); //���ȼ��������
};
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//
//									ͼģ���ࣺ����
//
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


template <typename Tv> struct Vertex 
{	//�������Ϊ���������δ�ϸ��װ��
	Tv data; int inDegree, outDegree; VStatus status; //���ݡ����������״̬
	int dTime, fTime; //ʱ���ǩ
	int parent; int priority; //�ڱ������еĸ��ڵ㡢���ȼ���
	Vertex(Tv const& d = (Tv)0) : //�����¶���
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {} //�ݲ�����Ȩ�����
};

template <typename Te> struct Edge 
{	//�߶���Ϊ���������δ�ϸ��װ��
	Te data; int weight; EStatus status; //���ݡ�Ȩ�ء�״̬
	Edge(Te const& d, int w) : data(d), weight(w), status(UNDETERMINED) {} //�����±�
};


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//
//						�������������ڽӾ�����ʽʵ�ְmͼ
//
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename Tv, typename Te> //�������͡�������
class GraphMatrix : public Graph<Tv, Te> 
{	//�������������ڽӾ�����ʽʵ�ֵ�ͼ
	private:
		Vector<Vertex<Tv>> V; //���㼯��������
		Vector<Vector<Edge<Te>*>> E; //�߼����ڽӾ���
	public:
		GraphMatrix() { n = e = 0; } //����
		~GraphMatrix() 
		{	//����
			for (int j = 0; j < n; j++) //���ж�̬������
				for (int k = 0; k < n; k++) //�߼�¼
					delete E[j][k]; //�������
		}

		// ����Ļ�����������ѯ��i�����㣨0 <= i < n��
		virtual Tv& vertex(int i) { return V[i].data; } //����
		virtual int inDegree(int i) { return V[i].inDegree; } //���
		virtual int outDegree(int i) { return V[i].outDegree; } //����
		virtual int firstNbr(int i) { return nextNbr(i, n); } //�׸��ڽӶ���
		virtual int nextNbr(int i, int j) //����ڶ���j����һ�ڽӶ���
			{ while ((-1 < j) && (!exists(i, --j))); return j; } //����������̽�������ڽӱ������Ч�ʣ�
		virtual VStatus& status(int i) { return V[i].status; } //״̬
		virtual int& dTime(int i) { return V[i].dTime; } //ʱ���ǩdTime
		virtual int& fTime(int i) { return V[i].fTime; } //ʱ���ǩfTime
		virtual int& parent(int i) { return V[i].parent; } //�ڱ������аm����
		virtual int& priority(int i) { return V[i].priority; } //�ڱ������е����ȼ���
		
		// ����Ķ�̬����
		virtual int insert(Tv const& vertex) 
		{	//���붥�㣬���ر��
			for (int j = 0; j < n; j++) E[j].insert(NULL); n++; //������Ԥ��һ��Ǳ�ڵĹ�����
			E.insert(Vector<Edge<Te>*>(n, n, (Edge<Te>*) NULL)); //�����¶����Ӧ�m������
			return V.insert(Vertex<Tv>(vertex)); //������������һ������
		}
		virtual Tv remove(int i) 
		{	//ɾ����i�����㼰������ߣ�0 <= i < n��
			for (int j = 0; j < n; j++) //���г���
				if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; } //����ɾ��
			E.remove(i); n--; //ɾ����i��
			for (int j = 0; j < n; j++) //���г���
				if (exists(j, i)) { delete E[j].remove(i); V[j].outDegree--; } //����ɾ��
			Tv vBak = vertex(i); V.remove(i); //ɾ������i
			return vBak; //���ر�ɾ���������Ϣ
		}

		// �ߵ�ȷ�ϲ���
		virtual bool exists(int i, int j) //��(i, j)�Ƿ����
		{ return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL; }
		
		// �ߵĻ�����������ѯ����i��j֮������ߣ�0 <= i, j < n��exists(i, j)��
		virtual EStatus& status(int i, int j) { return E[i][j]->status; } //��(i, j)��״̬
		virtual Te& edge(int i, int j) { return E[i][j]->data; } //��(i, j)������
		virtual int& weight(int i, int j) { return E[i][j]->weight; } //��(i, j)��Ȩ��
		
		// �ߵĶ�̬����
		virtual void insert(Te const& edge, int w, int i, int j) 
		{	//����Ȩ��Ϊw�ı�e = (i, j)
			if (exists(i, j)) return; //ȷ���ñ��в�����
			E[i][j] = new Edge<Te>(edge, w); //�����±�
			e++; V[i].outDegree++; V[j].inDegree++; //���±߼������������Ķ���
		}
		virtual Te remove(int i, int j) 
		{	//ɾ������i��j֮��m���ߣ�exists(i, j)��
			Te eBak = edge(i, j); delete E[i][j]; E[i][j] = NULL; //���ݺ�ɾ���߼�¼
			e--; V[i].outDegree--; V[j].inDegree--; //���±߼������������Ķ���
			return eBak; //���ر�ɾ���ߵ���Ϣ
		}
};
#endif