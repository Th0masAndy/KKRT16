#pragma once
// This file and the associated implementation has been placed in the public domain, waiving all copyright. No restrictions are placed on its use. 
#include "Common/Defines.h"
#include <array>
#include "Common/ArrayView.h"

//#include "cryptoTools/gsl/multi_span.h"
namespace bOPRF {


	template<class T>
	class MatrixView
	{
	public:

		using iterator = T*;
		using const_iterator = T*;
		using reverse_iterator = T*;
		using const_reverse_iterator = T*;
		//using iterator = gsl::span<T>::iterator;

		typedef T value_type;
		typedef value_type* pointer;
		typedef u64 size_type;


		MatrixView()
			:mStride(0)
		{
		}

		MatrixView(const MatrixView& av) :
			mView(av.mView),
			mStride(av.mStride)
		{ }

		MatrixView(pointer data, size_type numRows, size_type stride) :
			mView(data, numRows * stride),
			mStride(stride)
		{}

		MatrixView(pointer start, pointer end, size_type stride) :
			mView(start, end - ((end - start) % stride)),
			mStride(stride)
		{
		}

		template <class Iter>
		MatrixView(Iter start, Iter end, size_type stride, typename Iter::iterator_category *p = 0) :
			mView(start, end/* - ((end - start) % stride)*/),
			mStride(stride)
		{
			std::ignore = p;
		}

		template<template<typename, typename...> class C, typename... Args>
		MatrixView(const C<T, Args...>& cont, size_type stride, typename C<T, Args...>::value_type* p = 0) :
			MatrixView(cont.begin(), cont.end(), stride)
		{
			std::ignore = p;
		}

		const MatrixView<T>& operator=(const MatrixView<T>& copy)
		{
			mView = copy.mView;
			mStride = copy.mStride;
			return copy;
		}


		void reshape(size_type rows, size_type columns)
		{
			if (rows * columns != size())
				throw std::runtime_error(LOCATION);

			mView = ArrayView<T>(mView.data(), rows * columns);
			mStride = columns;
		}

		const size_type size() const { return mView.size(); }
		const size_type stride() const { return mStride; }

		std::array<size_type, 2> bounds() const { return { size() / stride() , stride() }; }

		pointer data() const { return mView.data(); };

		iterator begin() const { return mView.begin(); };
		iterator end() const { return mView.end(); }

		T& operator()(size_type rowIdx, size_type colIdx) const
		{
			if (colIdx >= stride()) throw std::runtime_error(LOCATION);
			return mView[rowIdx * stride() + colIdx];
		}

		ArrayView<T> operator[](size_type rowIdx) const
		{
#ifndef NDEBUG
		    if (rowIdx >= mView.size() / stride()) throw std::runtime_error(LOCATION);
#endif
		
		    return ArrayView<T>(mView.data() + rowIdx * stride(), stride());
		}
		


	protected:
		ArrayView<T> mView;
		size_type mStride;


	};

}