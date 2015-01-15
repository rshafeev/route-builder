// This is a samle program on C/C++ language.
// Demonstrate OOP
// Description of Matrix class.
// I used this class for my own graduate work, and temp paper.
#ifndef __matrix_h__
#define __matrix_h__
#include <math.h>
#include <vector>

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif

using namespace std;
//����� ���������� ���������
#define debug
class MATHLIB_API CMatrixException{
	char ne;
public:
	CMatrixException(char c){ne=c;}
	char* PrintMessage(){
		switch(ne){
		case 'a':return "���������� �������� ������";
		case '+':return "�������� �� ��������";
		case '-':return"��������� �� ��������";
		case '*':return"�������� �� ��������";
		case '/':return"������� ����������";
		case 'd':return"����������� � ��� double �� ��������";
		case '=':return"���������� ���������� �������";
		case '0':return"������� �� ����";
		case 'i':return"������������ ������";
		case 'T':return"��������������� ������� ��������� ����������";
		case 'I':return"��������� ������� ����������";
		default	:return"����������� ������";
		}
	}
};
class MATHLIB_API CMatrix 
{
private:
	int rows, cols;
	double* p;
	int matr_inv_gause(double *a, int n);
public:
	
	//�����������
	CMatrix(int, int);
	CMatrix(int, int,double,...);
	CMatrix(int, int,   int,...);
	CMatrix(double);
			// ������� ������� ������� �������� 1 � 1, m(0,0)=i
	CMatrix(int);
			// ������� ������� ������� �������� 1 � 1, m(0,0)=i
	CMatrix(CMatrix const &);
	//����������
	~CMatrix(){if(p!=NULL)delete[]p;}
	//
	CMatrix const& operator=(CMatrix const&);
	//����������� � ��� double
	operator double()const;
	//�������� ��������
	friend double operator+(double,CMatrix const&);
	double operator+(double d){return d+*this;}
	double operator+(   int i){return double(i)+*this;}
	friend double operator+( int i,CMatrix const&m){return double(i)+m;}
	CMatrix const& operator+()const{return*this;}//unary +
	CMatrix  operator+(CMatrix const&)const;//binary +
//	CMatrix& operator+=(CMatrix const&a){*this=*this+a;return*this;}

	//�������� ���������
	friend double operator-(double,CMatrix const&);
	double operator-(double d)const{return -(d-*this);}
	double operator-(int i)const{return *this-double(i);}
	friend double operator-(int i,CMatrix const&m){return -(m-i);}
	CMatrix  operator-(CMatrix const&)const;//binary -
	CMatrix& operator-=(CMatrix const&a){*this=*this-a;return*this;}
	CMatrix  operator-()const;//unary -
	//�������� ���������
	CMatrix& CMatrix::operator*=(CMatrix const&a){*this=(*this)*a;return*this;}
	CMatrix  operator*(double)const;
	friend CMatrix operator*(double d,CMatrix const&a){return a*d;}
	CMatrix& operator*=(double d){*this=*this*d;return*this;}
	CMatrix operator*(CMatrix const&)const;
	//Division operators
	CMatrix  operator/(double d)const{
		if(d==0)throw CMatrixException('d');
		return (1./d)*(*this);
	}
	CMatrix& operator/=(double d){*this=*this/d;return*this;}
	//Transposition operator
	CMatrix  operator*()const;
//	friend Matrix T(Matrix const&a){return *a;}
	//������ � ��������� �������
	double& CMatrix::operator()(int i, int j)const;
	//���������������� �������
	CMatrix T(CMatrix const&a)const;
	//��������� �������
	CMatrix Inv(CMatrix &a);
	CMatrix Pow(int n);
	double Determ();
	vector<double> GetCharacteristicPolinomValues();//������������������ ���������
    vector<double> GetEigenValues();
	double GetSpMatrix();//���� �������
	
	double GetNorma1();
	double GetNorma2();
//������� ���-�� �������� � ����� �������
	int GetRows()const{return rows;}
	int GetCols()const{return cols;}
	void SetElem(int m,int n,double value);
	void SetElem(int ind,double value);
	void WriteToFile(char* FileName,bool ReWrite);
};
#endif


