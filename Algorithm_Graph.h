#ifndef ALGORITHM_GRAPH_H_
#define ALGORITHM_GRAPH_H_
#include "Algorithm_Vector.h" //引入向量

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus; //边状态

using namespace std;
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//
//										图模板类
//
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
template <typename Tv,typename Te> //顶点类型、边类型
class Graph 
{	//图Graph模板类
	private:
		void reset() 
		{	//所有顶点、边的辅助信息复位
			for (int i = 0; i < n; i++) 
			{	
				status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1; //所有顶点的状态，时间标签
				parent(i) = -1; priority(i) = INT_MAX; //（在遍历树中的）父节点，优先级数
				for (int j = 0; j < n; j++) 
					if (exists(i, j)) status(i, j) = UNDETERMINED; //所有边癿状态
			}
		}
		void BFS(int, int&); //（连通域）广度优先搜索算法
		void DFS(int, int&); //（连通域）深度优先搜索算法
		void BCC(int, int&, Stack<int>&); //（连通域）基于DFS癿双连通分量分解算法
		bool TSort(int, int&, Stack<Tv>*); //（连通域）基与DFS的拓扑排序算法
		template <typename PU> void PFS(int, PU); //（连通域）优先级搜索框架
	public:
		// 顶点
		int n; //顶点总数
		virtual int insert(Tv const&) = 0; //插入顶点，返回编号
		virtual Tv remove(int) = 0; //删除顶点及其关联边，返回其顶点信息
		virtual Tv& vertex(int) = 0; //顶点v的数据（该顶点癿确存在）
		virtual int inDegree(int) = 0; //顶点v的入度（该顶点癿确存在）
		virtual int outDegree(int) = 0; //顶点v的出度（该顶点癿确存在）
		virtual int firstNbr(int) = 0; //顶点v的首个邻接顶点
		virtual int nextNbr(int, int) = 0; //顶点v的（相对于顶点j的）下一邻接顶点
		virtual VStatus& status(int) = 0; //顶点v的状态
		virtual int& dTime(int) = 0; //顶点v的时间标签dTime
		virtual int& fTime(int) = 0; //顶点v的时间标签fTime
		virtual int& parent(int) = 0; //顶点v在遍历树中的父亲
		virtual int& priority(int) = 0; //顶点v在遍历树中的优先级数
		// 边：返里约定，无向边均统一转化为斱向互逆癿一对有向边，从而将无向图规作有向图癿特例
		int e; //边总数
		virtual bool exists(int, int) = 0; //边(v, u)是否存在
		virtual void insert(Te const&, int, int, int) = 0; //在顶点v和u之间插入权重为w的边e
		virtual Te remove(int, int) = 0; //删除顶点v和u之间的边e，返回该边信息
		virtual EStatus& status(int, int) = 0; //边(v, u)的状态
		virtual Te& edge(int, int) = 0; //边(v, u)的数据（该边的确存在）
		virtual int& weight(int, int) = 0; //边(v, u)的权重
		// 算法
		void bfs(int); //广度优先搜索算法
		void dfs(int); //深度优先搜索算法
		void bcc(int); //基亍DFS的双连通分量分解算法
		Stack<Tv>* tSort(int); //基亍DFS的拓扑排序算法
		void prim(int); //最小支撑树Prim算法
		void dijkstra(int); //最短路径Dijkstra算法
		template <typename PU> void pfs(int, PU); //优先级搜索框架
};
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//
//									图模板类：结束
//
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


template <typename Tv> struct Vertex 
{	//顶点对象（为简化起见，并未严格封装）
	Tv data; int inDegree, outDegree; VStatus status; //数据、出入度数、状态
	int dTime, fTime; //时间标签
	int parent; int priority; //在遍历树中的父节点、优先级数
	Vertex(Tv const& d = (Tv)0) : //构造新顶点
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {} //暂不考虑权重溢出
};

template <typename Te> struct Edge 
{	//边对象（为简化起见，并未严格封装）
	Te data; int weight; EStatus status; //数据、权重、状态
	Edge(Te const& d, int w) : data(d), weight(w), status(UNDETERMINED) {} //构造新边
};


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//
//						基于向量，以邻接矩阵形式实现癿图
//
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename Tv, typename Te> //顶点类型、边类型
class GraphMatrix : public Graph<Tv, Te> 
{	//基于向量，以邻接矩阵形式实现的图
	private:
		Vector<Vertex<Tv>> V; //顶点集（向量）
		Vector<Vector<Edge<Te>*>> E; //边集（邻接矩阵）
	public:
		GraphMatrix() { n = e = 0; } //极造
		~GraphMatrix() 
		{	//析构
			for (int j = 0; j < n; j++) //所有动态创建的
				for (int k = 0; k < n; k++) //边记录
					delete E[j][k]; //逐条清除
		}

		// 顶点的基本操作：查询第i个顶点（0 <= i < n）
		virtual Tv& vertex(int i) { return V[i].data; } //数据
		virtual int inDegree(int i) { return V[i].inDegree; } //入度
		virtual int outDegree(int i) { return V[i].outDegree; } //出度
		virtual int firstNbr(int i) { return nextNbr(i, n); } //首个邻接顶点
		virtual int nextNbr(int i, int j) //相对于顶点j的下一邻接顶点
			{ while ((-1 < j) && (!exists(i, --j))); return j; } //逆向线性试探（改用邻接表可提高效率）
		virtual VStatus& status(int i) { return V[i].status; } //状态
		virtual int& dTime(int i) { return V[i].dTime; } //时间标签dTime
		virtual int& fTime(int i) { return V[i].fTime; } //时间标签fTime
		virtual int& parent(int i) { return V[i].parent; } //在遍历树中癿父亲
		virtual int& priority(int i) { return V[i].priority; } //在遍历树中的优先级数
		
		// 顶点的动态操作
		virtual int insert(Tv const& vertex) 
		{	//插入顶点，返回编号
			for (int j = 0; j < n; j++) E[j].insert(NULL); n++; //各顶点预留一条潜在的关联边
			E.insert(Vector<Edge<Te>*>(n, n, (Edge<Te>*) NULL)); //创建新顶点对应癿边向量
			return V.insert(Vertex<Tv>(vertex)); //顶点向量增加一个顶点
		}
		virtual Tv remove(int i) 
		{	//删除第i个顶点及其关联边（0 <= i < n）
			for (int j = 0; j < n; j++) //所有出边
				if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; } //逐条删除
			E.remove(i); n--; //删除第i行
			for (int j = 0; j < n; j++) //所有出边
				if (exists(j, i)) { delete E[j].remove(i); V[j].outDegree--; } //逐条删除
			Tv vBak = vertex(i); V.remove(i); //删除顶点i
			return vBak; //返回被删除顶点的信息
		}

		// 边的确认操作
		virtual bool exists(int i, int j) //边(i, j)是否存在
		{ return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL; }
		
		// 边的基本操作：查询顶点i与j之间的联边（0 <= i, j < n且exists(i, j)）
		virtual EStatus& status(int i, int j) { return E[i][j]->status; } //边(i, j)的状态
		virtual Te& edge(int i, int j) { return E[i][j]->data; } //边(i, j)的数据
		virtual int& weight(int i, int j) { return E[i][j]->weight; } //边(i, j)的权重
		
		// 边的动态操作
		virtual void insert(Te const& edge, int w, int i, int j) 
		{	//插入权重为w的边e = (i, j)
			if (exists(i, j)) return; //确保该边尚不存在
			E[i][j] = new Edge<Te>(edge, w); //创建新边
			e++; V[i].outDegree++; V[j].inDegree++; //更新边计数与关联顶点的度数
		}
		virtual Te remove(int i, int j) 
		{	//删除顶点i和j之间癿联边（exists(i, j)）
			Te eBak = edge(i, j); delete E[i][j]; E[i][j] = NULL; //备份后删除边记录
			e--; V[i].outDegree--; V[j].inDegree--; //更新边计数与关联顶点的度数
			return eBak; //返回被删除边的信息
		}
};
#endif
