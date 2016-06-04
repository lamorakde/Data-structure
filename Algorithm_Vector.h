#ifndef ALGORITHM_VECTOR_H_
#define ALGORITHM_VECTOR_H_

typedef int Rank; // ��

template <typename T> class Vector
{
protected:
	Rank _size; int _capacity; T * _elem; //��ģ��������������
	void copyFrom(T const* A, Rank lo, Rank hi); //������������A[lo, hi)
	void expand(); //�ռ䲻��ʱ����
	void shrink(); //װ�����ӹ�Сʱѹ��
	bool bubble(Rank lo, Rank hi); //ɨ�轻��
	void bubbleSort(Rank lo, Rank hi); //���������㷨
	Rank max(Rank lo, Rank hi); //ѡ�����Ԫ��
	void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
	void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
	void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
	Rank partition(Rank lo, Rank hi); //��㼫���㷨
	void quickSort(Rank lo, Rank hi); //���������㷨
	void heapSort(Rank lo, Rank hi); //�������Ժ�����ȫ�ѽ��⣩

public:
	// ���캯��
	Vector(int c = 3, int s = 0, T v = 0) { _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v) {} } //����Ϊc����ģΪs������Ԫ�س�ʼΪv
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //�������临��
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //�������帴��
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //�������临��
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //�������帴�c
																  // ��������
	~Vector() { delete[] _elem; } //�ͷ��ڲ��ռ�
								  // ����ڪ�ʽӿ�
	Rank size() const { return _size; } //��ģ
	bool empty() const { return !_size; } //�ж��Ƿ�Ϊ��
	int disordered() const; //�`�������Ƿ�������
	Rank find(T const& e) const { return find(e, 0, (Rank)_size); } //���������������
	Rank find(T const& e, Rank lo, Rank hi) const; //���������׼����
	Rank search(T const& e) const //���������������
	{
		return (0 >= _size) ? -1 : search(e, (Rank)0, (Rank)_size);
	}
	Rank search(T const& e, Rank lo, Rank hi) const; //���������׼����
													 // ��д���ʽӿ�
	T& operator[](Rank r) const; //�����±����������������ء������ʽ���ø�Ԫ��
	Vector<T> & operator=(Vector<T> const&); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
	T remove(Rank r); //������Ϊr�mԪ��
	int remove(Rank lo, Rank hi); //ɾ����������[lo, hi)֮�ڵ�Ԫ��
	Rank insert(Rank r, T const& e); //����Ԫ��
	Rank insert(T const& e) { return insert(_size, e); } //Ĭ����ΪĩԪ�ز���
	void sort(Rank lo, Rank hi); //��[lo, hi)����
	void sort() { sort(0, _size); } //��������
	void unsort(Rank lo, Rank hi); //��[lo, hi)����
	void unsort() { unsort(0, _size); } //��������
	int deduplicate(); //����ȥ��
	int uniquify(); //����ȥ��
					// ����
	void traverse(void(*visit)(T&)); //������ʹ�ú���ָ�룬�����������޸ģ�
	template <typename VST> void traverse(VST&); //������ʹ�ú������󣬿�ȫ�����޸ģ�
};

template < typename T> //Ԫ������
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)
{	//������ռ�A[lo, hi)Ϊ������������
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0; //����ռ䣬��ģ����
	while (lo < hi)
	{
		//A[lo, hi)�ڵ�Ԫ����һ������_elem[0, hi - lo)
		_elem[_size++] = A[lo++];
	}
}

template <typename T> void Vector<T>::expand() { //�����ռ䲻��ʱ����
	if (_size < _capacity) return; //��δ��Աʱ����������
	if (_capacity < 3) _capacity = 3; //��������С����
	T* oldElem = _elem; _elem = new T[_capacity <<= 1]; //�����ӱ�
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; //����ԭ�������ݣ�TΪ�������ͣ��������ظ�ֵ������'='��
	delete[] oldElem; //�ͷ�ԭ�ռ�
}

template <typename T> void Vector<T>::shrink() { //װ�����ӹ�Сʱѹ��������ռ�ռ�
	if (_capacity < 3 << 1) return; //����������DEFAULT_CAPACITY����
	if (_size << 2 > _capacity) return; //��25%Ϊ��
	T* oldElem = _elem; _elem = new T[_capacity >>= 1]; //��������
	for (int i = 0; i < _size; i++) _elem[i] = oldElem[i]; //����ԭ��������
	delete[] oldElem; //�ͷ�ԭ�ռ�
}

template <typename T> T& Vector<T>::operator[](Rank r) const //�����±������
{
	return _elem[r];
} // assert: 0 <= r < _size

template <typename T> void permute(Vector<T>& V) { //�������������ʹ��Ԫ�صȸ��ʳ���ء��һλ��
	for (int i = V.size(); i > 0; i--) //�Ժ���ǰ
		swap(V[i - 1], V[rand() % i]); //V[i - 1]��V[0, i)��ĳһ���Ԫ�ؽ���
}

template <typename T> void Vector<T>::unsort(Rank lo, Rank hi)
{
	//�ȸ������������������[lo, hi)
	T* V = _elem + lo; //��������_elem[lo, hi)������һ����V[0, hi - lo)
	for (Rank i = hi - lo; i > 0; i--) //�Ժ���ǰ
		swap(V[i - 1], V[rand() % i]); //��V[i - 1]��V[0, i)��ĳһԪ���������
}

template < typename T> //���������m������ң��������һ��Ԫ��e��λ�ã�ʧ��ʱ������lo - 1
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const
{	//assert: 0 <= lo < hi <= _size
	while ((lo < hi--) && (e != _elem[hi])); //�Ӻ���ǰ���������
	return hi; //��hi < lo������ζ��ʧ�ܣ�����hi������Ԫ�ص���
}

template < typename T> //��e��Ϊ��ΪrԪ�ز���
Rank Vector<T>::insert(Rank r, T const& e) { //assert: 0 <= r <= size
	expand(); //���б�Ҫ������
	for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1]; //�Ժ���ǰ�����Ԫ��˳�κ��һ����Ԫ
	_elem[r] = e; _size++; //������Ԫ�ز���������
	return r; //������
}

template <typename T> int Vector<T>::remove(Rank lo, Rank hi) { //ɾ������[lo, hi)
	if (lo == hi) return 0; //��ءЧ�ʿ��ǣ����������˻����������remove(0, 0)
	while (hi < _size) _elem[lo++] = _elem[hi++]; //[hi, _size)˳��ǰ�� hi - lo ����Ԫ
	_size = lo; //���¹�ģ��ֱ�Ӷ���β��[lo, _size = hi)����
	shrink();	//���б�Ҫ��������
	return hi - lo; //���ر�ɾ��Ԫ�ص���Ŀ
}

template <typename T> T Vector<T>::remove(Rank r) { //������������Ϊr�mԪ�أ�0 <= r < size
	T e = _elem[r]; //���ݱ�ɾ��Ԫ��
	remove(r, r + 1); //������������㷨����Ч�ڶ�����[r, r + 1)��ɾ��
	return e; //���ر�ɾ��Ԫ��
}

template <typename T> int Vector<T>::deduplicate() { //ɾ�������������ظ�Ԫ�أ���Ч�棩
	int oldSize = _size; //��¼ԭʼ��ģ
	Rank i = 1; //��_elem[1]��ʼ
	while (i < _size) //��ǰ�����һ�����Ԫ��_elem[i]
		(find(_elem[i], 0, i) < 0) ? //����ǰ׺��Ѱ����֮��ͬ�ߣ�����һ����
		i++ : remove(i); //������ͬ������������̣�����ɾ����ͬ��
	return oldSize - _size; //������ģ�仯��������ɾ��Ԫ������
}

template <typename T> void Vector<T>::traverse(void(*visit)(T&)) //���ú���ָ����Ƶı���
{
	for (int i = 0; i < _size; i++) visit(_elem[i]);
}

template <typename T> template <typename VST> //Ԫ�����͡�������
void Vector<T>::traverse(VST& visit) //���ú���������Ƶı���
{
	for (int i = 0; i < _size; i++) visit(_elem[i]);
}

template <typename T> struct Increase //�������󣺵���һ��T�����
{
	virtual void operator()(T& e) { e++; }
}; //����T��ֱ�ӵ�����������++

template <typename T> void increase(Vector<T> & V) //ͳһ���������еĸ�Ԫ��
{
	V.traverse(Increase<T>());
} //��Increase<T>()Ϊ�����������б���

template <typename T> int Vector<T>::disordered() const
{	//������������������Ԫ�ضԵ�����
	int n = 0; //������
	for (int i = 1; i < _size; i++) //��һ���_size - 1������Ԫ��
		if (_elem[i - 1] > _elem[i]) n++; //���������
	return n; //���������ҽ��� n = 0
}

// template <typename T> int Vector<T>::uniquify() 
// {	//���������ظ�Ԫ���޳��㷨����Ч�棩
// 	int oldSize = _size; int i = 1; //��ǰ�ȶ�Ԫ�ص��ȣ���ʼ����Ԫ��
// 	while (i < _size) //��ǰ�����һ�ȶԸ�������Ԫ��
// 		_elem[i - 1] == _elem[i] ? remove(i) : i++; //����ͬ����ɾ�����ߣ�����ת����һԪ��
// 	return oldSize - _size; //������ģ�仯��������ɾ��Ԫ������
// }

template <typename T> int Vector<T>::uniquify()
{	//���������ظ�Ԫ���޳��㷨����Ч�棩
	Rank i = 0, j = 0; //���Ի��조���ڡ�Ԫ�ذm��
	while (++j < _size) //��һɨ�裬ֱ��ĩԪ��
		if (_elem[i] != _elem[j]) //������ͬ��
			_elem[++i] = _elem[j]; //������ͬԪ��ʱ����ǰ����������ǰ���Ҳ�
	_size = ++i; shrink(); //ֱ�ӽس�β������Ԫ��
	return j - i; //������ģ�仯������������Ԫ������
}

template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{	//�����������m���� [lo, hi) �ڣ�ȷ�������� e �����һ���ڵ����
	return (rand % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi); //���� 50% �ĸ������ʹ�ö��ֲ��һ��� Fibonacci ����
}

// ���ֲ����㷨���汾C�������������������� [lo, hi) �ڲ���Ԫ��e ��0 <= lo <= hi <= _size
template <typename T> static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {
	while (lo < hi) { //ÿ������������һ�αȽ��жϣ���������֧
		Rank mi = (lo + hi) >> 1; //���е�Ϊ���
		(e < A[mi]) ? hi = mi : lo = mi + 1; //���ȽϺ�ȷ������ [lo, mi) �� (mi, hi)
	} //�ɹ����Ҳ�����ǰ��ֹ
	return --lo; //ѭ������ʱ��loΪ����e��Ԫ�ص���С�ȣ���lo - 1��������e��Ԫ�ص������
				 //�ж������Ԫ��ʱ�����ܱ�֤����������ߣ�����ʧ��ʱ���ܹ�����ʧ�ܵ�λ��
}

// Fibonacci�����㷨���汾A�������������������� [lo, hi) �ڲ���Ԫ�� e��0 <= lo <= hi <= _size
template <typename T> static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi) {
	Fib fib(hi - lo); //��O(log_phi(n = hi - lo)ʱ�䴴��Fib����
	while (lo < hi) { //�𲽵�������Ҫ�����αȽ��жϣ���������֧
		while (hi - lo < fib.get()) fib.prev(); //ͨ����ǰ������ң���̯O(1)��
		Rank mi = lo + fib.get() - 1; //ȷ������ Fib(k) - 1 �����
		if (e < A[mi]) hi = mi; //����ǰ�벿[lo, mi)��������
		else if (A[mi] < e) lo = mi + 1; //�����벿(mi, hi)��������
		else return mi; //�� mi ������
	} //�ɹ����ҿ�����ǰ��ֹ
	return -1; //����ʧ��
			   //�ж������Ԫ��ʱ�����ܱ�֤����������ߣ�ʧ��ʱ���򵥵ط���-1��������ָʾʧ�ܵ�λ��
}

template <typename T> void Vector<T>::sort(Rank lo, Rank hi) { //�������� [lo, hi) ����
	switch (rand() % 5) { //���ѡ�������㷨���ɸ��ݾ���������ص����ѡ�������
	case 1: bubbleSort(lo, hi); break; //��������
	case 2: selectionSort(lo, hi); break; //ѡ������
	case 3: mergeSort(lo, hi); break; //�鲢����
	case 4: heapSort(lo, hi); break; //������
	default: quickSort(lo, hi); break; //��������
	}
}

template < typename T> //��������������
void Vector<T>::bubbleSort(Rank lo, Rank hi) //assert: 0 <= lo < hi <= size
{
	while (!bubble(lo, hi--));
} //������ɨ�轻����ֱ��ȫ��

template <typename T> bool Vector<T>::bubble(Rank lo, Rank hi) { //һ��ɨ�轻��
	bool sorted = true; //���������־
	while (++lo < hi) //�������ң���һ����������Ԫ��
		if (_elem[lo - 1] > _elem[lo]) { //��������
			sorted = false; //��ζ����δ�������򣬲���Ҫ
			swap(_elem[lo - 1], _elem[lo]); //ͨ������ʹ�ֲ�����
		}
	return sorted; //���������־
}

template < typename T> //�����鲢����
void Vector<T>::mergeSort(Rank lo, Rank hi)   //0 <= lo < hi <= size
{
	if (hi - lo < 2) return; //��Ԫ��������Ȼ���򣬷���...
	int mi = (lo + hi) >> 1; //���е�Ϊ��
	mergeSort(lo, mi);
	mergeSort(mi, hi);
	merge(lo, mi, hi); //�ֱ��ǰ����������Ȼ��鲢
}

template < typename T> //���������Ĺ鲢
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) { //��miΪ�硢���������������[lo, mi)��[mi, hi)
	T* A = _elem + lo; //�ϲ��������A[0, hi - lo) = _elem[lo, hi)
	int lb = mi - lo; T* B = new T[lb]; //ǰ������B[0, lb) = _elem[lo, mi)
	for (Rank i = 0; i < lb; B[i] = A[i++]); //����ǰ������
	int lc = hi - mi; T* C = _elem + mi; //��������C[0, lc) = _elem[mi, hi)
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) { //��B[j]��C[k]�е�С������Aĩβ
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] < B[j]))) A[i++] = C[k++];
	}
	delete[] B; //�ͷ���ʱ�ռ�B
				//�鲢��õ���������������[lo, hi)
}

#endif