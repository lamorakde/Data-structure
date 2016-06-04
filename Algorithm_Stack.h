#ifndef ALGORITHM_STACK_H_
#define ALGORITHM_STACK_H_

#include "Algorithm_Vector.h" //������Ϊ���࣬������ջģ����

template <typename T> class Stack : public Vector<T> 
{	//����������/ĩ����Ϊջ��/��
	public: //size()�� empty()�Լ��������Žӿڣ�����ֱ������
		void push(T const& e) { insert(size(), e); } //��ջ����Ч�ڽ���Ԫ����Ϊ������ĩԪ�ز���
		T pop() { return remove(size() - 1); } //��ջ����Ч��ɾ��������ĩԪ��
		T& top() { return (*this)[size() - 1]; } //ȡ����ֱ�ӷ���������ĩԪ��
};

#endif
