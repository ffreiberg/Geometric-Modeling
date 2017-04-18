////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#ifndef _VEC_H_
#define _VEC_H_

//#include <iostream>
#include "math.h"

template<class T, unsigned SIZE> class CMatrix;

// Vector class for SIMPLE data types
template<class T, unsigned SIZE> class CVector 
{
private:
	T m_atData[SIZE];

public:
	// constructors
	CVector<T, SIZE> (                                      ); // default constructor
	CVector<T, SIZE> (const         T           atData[SIZE]); // construction with data array
	CVector<T, SIZE> (const CVector<T, SIZE  > &vec         ); // copy constructor
	CVector<T, SIZE> (const CVector<T, SIZE+1> &vec         ); // cast constructor
	CVector<T, SIZE> (const CVector<T, SIZE+2> &vec         ); // cast constructor
	CVector<T, SIZE> (T x1, T x2, T x3, T x4                ); // simple constructor

	// destructor
	~CVector<T, SIZE> (); 

	// getter-/setter-related methods
	void     setData      (const T       atData[SIZE]);
	void     getData      (      T       atData[SIZE]);
	unsigned getDimension (                          ) const;
	void     swap         (unsigned i=0, unsigned j=1);  // swaps i-tj and j-th coordinate         

	// norm-related methods
	T    norm2     () const;	// squared norm of vector
	T    norm      () const;	// norm of vector
	void homogenize();			// homogenize CVec3+Hom
	void normalize ();			// normalize vector

	// assignment operators
	CVector<T, SIZE> &operator = (const CVector<T, SIZE> &vec         );
	CVector<T, SIZE> &operator = (const         T         atData[SIZE]);

	// index operators
	T &operator () (unsigned i);			// usage of operator:	vec(i) = var;	// 0 <= i <= SIZE-1
	T  operator () (unsigned i) const ;		//						var = vec(i);   vec1(i) = vec2(j); 

	// arithmetic operators
	void             operator += (const CVector<T, SIZE> &vec); // add vector
	void             operator -= (const CVector<T, SIZE> &vec); // subtract 
	void             operator *= (const         T         t  ); // multiply with scalar
	CVector<T, SIZE> operator +  (const CVector<T, SIZE> &vec); // sum of vectors
	CVector<T, SIZE> operator -  (const CVector<T, SIZE> &vec); // difference of vectors
	CVector<T, SIZE> operator -  (                           ); // sign
	CVector<T, SIZE> operator *  (const         T         t  ); // product of scalar and vector
	CVector<T, SIZE> operator *  (const CMatrix<T, SIZE> &mat); // matrix-vector product
	T                operator *  (const CVector<T, SIZE> &vec); // scalar-product	
	CVector<T, 4>    cross       (const CVector<T, 4   > &vec); // cross-product for CVec3+Hom: Hom is ignored and set to 0.
	CVector<T, 3>    cross       (const CVector<T, 3   > &vec); // cross-product for CVec3.

	// output
	void print(char* name= "v") const;

}; // class CVector

// some common vector classes (abbr. names)
typedef CVector<float, 2> CVec2f;
typedef CVector<float, 3> CVec3f;
typedef CVector<float, 4> CVec4f;

typedef CVector<double, 2> CVec2d;
typedef CVector<double, 3> CVec3d;
typedef CVector<double, 4> CVec4d;


// standard constructor
template <class T, unsigned SIZE> 
CVector<T,SIZE>::CVector ()  
{	// initialize all elements with zero
	for (int i=0; i<SIZE; i++) m_atData[i] = T(0);
}

// construction with data array	
template <class T, unsigned SIZE> 
CVector<T,SIZE>::CVector (const T atData[SIZE]) 
{
	for (int i=0; i<SIZE; i++) m_atData[i] = atData[i];		
}

// copy constructor
template <class T, unsigned SIZE> 
CVector<T,SIZE>::CVector (const CVector<T,SIZE> &vec) 
{
	if (this==&vec) return; // nothing to do, it's me
	*this = vec;
}

// cast constructor
template <class T, unsigned SIZE> 
CVector<T,SIZE>::CVector (const CVector<T,SIZE+1> &vec) 
{
	for (int i=0; i<SIZE; i++) m_atData[i] = vec(i);
}

// cast constructor
template <class T, unsigned SIZE> 
CVector<T,SIZE>::CVector (const CVector<T,SIZE+2> &vec) 
{
	for (int i=0; i<SIZE; i++) m_atData[i] = vec(i);
}

// simple constructor
template <class T, unsigned SIZE> 
CVector<T,SIZE>::CVector (T x1, T x2, T x3, T x4) 
{
	if (SIZE == 4) {
		m_atData[0] = x1;
		m_atData[1] = x2;
		m_atData[2] = x3;
		m_atData[3] = x4;
	}
}

// destructor
template <class T, unsigned SIZE> 
CVector<T,SIZE>::~CVector () 
{	// nothing to do here ...
}

// setter and getter methods
template <class T, unsigned SIZE> 
void CVector<T,SIZE>::setData (const T atData[SIZE]) 
{
	for (int i=0; i<SIZE; i++) m_atData[i] = atData[i];		
}

template <class T, unsigned SIZE> 
void CVector<T,SIZE>::getData (T atData[SIZE]) 
{
	for (int i=0; i<SIZE; i++) atData[i] = m_atData[i];		
}

template <class T, unsigned SIZE> 
unsigned CVector<T,SIZE>::getDimension () const
{
	return SIZE;
}

template <class T, unsigned SIZE> 
void CVector<T,SIZE>::swap(unsigned i=0, unsigned j=1) // swaps i-th and j-th coordinates
{ 
	if (i!=j && i<SIZE && j<SIZE) {
		T tmp       = m_atData[i]; 
		m_atData[i] = m_atData[j]; 
		m_atData[j] = tmp; 
	}
}

// norm-related methods
template <class T, unsigned SIZE>	
T CVector<T,SIZE>::norm2() const 
{
	T d= 0;
	for (int i=0; i < SIZE; i++) d += m_atData[i] * m_atData[i];
	return d;
}

template <class T, unsigned SIZE>	
T CVector<T,SIZE>::norm() const 
{
	return sqrt(this->norm2());
}
	
// Norm: nur für CVec3+Hom definiert!
// Homogene Koordinate wird zunaechst auf 1 normiert, 
// anschliessend werden die uebrigen koordinaten als CVec3 auf 1 normiert.
template <class T, unsigned SIZE>	
void CVector<T,SIZE>::homogenize () 
{
	T atBuffer[4];
	// hom 1
	T d = (m_atData[3]==0)? T(1): m_atData[3];
	for (int i=0; i<4; i++)	m_atData[i] = m_atData[i] / d;
	// laenge 1
	for (int i=0; i<3; i++)	d += m_atData[i]*m_atData[i];
	d = sqrt(d);
	// Normalize vector
	for (int i=0; i<3; i++) m_atData[i] = m_atData[i] / d;
}

// Normiert die Laenge eines beliebigen Vektors auf 1.
template <class T, unsigned SIZE>	
void CVector<T,SIZE>::normalize () 
{
	T d = norm();
	// Normalize vector
	for (int i=0; i<SIZE; i++) m_atData[i] = m_atData[i] / d;
}

// assignment operators
template <class T, unsigned SIZE>	
CVector<T, SIZE> &CVector<T,SIZE>::operator = (const CVector<T, SIZE> &vec) 
{
	if (this==&vec)	return (*this); // ok, it's me, so no L-value action
	// not me, so L-Value action: copy data
	for (int i=0; i<SIZE; i++) m_atData[i] = vec(i);
	return (*this); 
}

template <class T, unsigned SIZE>	
CVector<T, SIZE> &CVector<T,SIZE>::operator = (const T atData[SIZE]) 
{
	// not me, so L-Value action: copy data
	for (int i=0; i<SIZE; i++) m_atData[i] = atData[i];
	return (*this);
}
	
// index operators
template <class T, unsigned SIZE>				// usage of operator:
T &CVector<T,SIZE>::operator () (unsigned i)	// vec(i) = var;	// 0 <= i <= SIZE-1
{												// var = vec(i);
	return m_atData[i%SIZE];					// vec1(i) = vec2(j);
}

template <class T, unsigned SIZE>	
T CVector<T,SIZE>::operator () (unsigned i) const 
{
	return m_atData[i%SIZE];
}

// arithmetic operators
template <class T, unsigned SIZE>	
void CVector<T,SIZE>::operator += (const CVector<T, SIZE> &vec) 
{
	for (int i=0; i<SIZE; i++) m_atData[i] += vec(i);
}

template <class T, unsigned SIZE>	
void CVector<T,SIZE>::operator -= (const CVector<T, SIZE> &vec) 
{
	for (int i=0; i<SIZE; i++) m_atData[i] -= vec(i);
}

template <class T, unsigned SIZE>	
CVector<T, SIZE> CVector<T,SIZE>::operator + (const CVector<T, SIZE> &vec) 
{
	CVector<T, SIZE> buf (m_atData);
	buf += vec;
	return buf;
}
	
template <class T, unsigned SIZE>	
CVector<T, SIZE> CVector<T,SIZE>::operator - (const CVector<T, SIZE> &vec) 
{
	CVector<T, SIZE> buf(m_atData);
	buf -= vec;
	return buf;
}
	
template <class T, unsigned SIZE>	
CVector<T, SIZE> CVector<T,SIZE>::operator - () 
{
	T atBuffer[SIZE];
	for (int i=0; i<SIZE; i++) atBuffer[i] = -m_atData[i];
	return CVector<T, SIZE> (atBuffer);
}

template <class T, unsigned SIZE>	
T CVector<T,SIZE>::operator * (const CVector<T, SIZE> & vec) 
{
	T d = T(0);
	for (int i=0; i<SIZE; i++) d += m_atData[i]*vec(i);
	return d;
}

template <class T, unsigned SIZE>	
void CVector<T,SIZE>::operator *= (const T tScale) 
{
	for (int i=0; i<SIZE; i++) m_atData[i] *= tScale;
}

template <class T, unsigned SIZE>	
CVector<T, SIZE> CVector<T,SIZE>::operator * (const T tScale) 
{
	CVector<T, SIZE> buf(m_atData);
	buf *= tScale;
	return buf;
}

template <class T, unsigned SIZE>	
CVector<T, SIZE> CVector<T,SIZE>::operator * (const CMatrix<T, SIZE> &mat) 
{
	CVector<T, SIZE> vec;
	for (int j=0; j<SIZE; j++)
		for (int i=0; i<SIZE; i++)
			vec(j) += m_atData[i]*mat(i,j);
	return vec;
}

// Kreuzprodukt: nur für CVec3+Hom definiert!
// Homogene Koordinate wird ignoriert und auf 1 gesetzt.
template <class T, unsigned SIZE>	
CVector<T, 4> CVector<T,SIZE>::cross (const CVector<T, 4> &vec) 
{
	T atBuffer[4];
	atBuffer[0] = m_atData[1] * vec(2);
	atBuffer[0]-= m_atData[2] * vec(1);
	atBuffer[1] = m_atData[2] * vec(0);
	atBuffer[1]-= m_atData[0] * vec(2);
	atBuffer[2] = m_atData[0] * vec(1);
	atBuffer[2]-= m_atData[1] * vec(0);
	atBuffer[3] = T(0);
	return CVector<T, 4> (atBuffer);
}
	
// Kreuzprodukt: nur für CVec3 definiert!
template <class T, unsigned SIZE>	
CVector<T, 3> CVector<T,SIZE>::cross (const CVector<T, 3> &vec) 
{
	T atBuffer[3];
	atBuffer[0] = m_atData[1] * vec(2);
	atBuffer[0]-= m_atData[2] * vec(1);
	atBuffer[1] = m_atData[2] * vec(0);
	atBuffer[1]-= m_atData[0] * vec(2);
	atBuffer[2] = m_atData[0] * vec(1);
	atBuffer[2]-= m_atData[1] * vec(0);
	return CVector<T, 3> (atBuffer);
}

template <class T, unsigned SIZE>	
void CVector<T,SIZE>::print(char* name= "v") const
{
	std::cout << name << " = (";
	for (int i=0; i<SIZE; i++) {
		std::cout << m_atData[i];
		if (i<SIZE-1) std::cout << ", ";
		else          std::cout << ")\n"; 
	}
}
	

#endif
